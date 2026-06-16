/*
 * Open Chinese Convert
 *
 * Copyright 2010-2014 BYVoid <byvoid@byvoid.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "BinaryDict.hpp"
#include "Lexicon.hpp"

using namespace opencc;

size_t BinaryDict::KeyMaxLength() const {
  size_t maxLength = 0;
  for (const DictEntry* entry : *lexicon) {
    maxLength = std::max(maxLength, entry->KeyLength());
  }
  return maxLength;
}

void BinaryDict::SerializeToFile(FILE* fp) const {
  string keyBuf, valueBuf;
  vector<size_t> keyOffsets, valueOffsets;
  size_t keyTotalLength = 0, valueTotalLength = 0;
  ConstructBuffer(keyBuf, keyOffsets, keyTotalLength, valueBuf,
                  valueOffsets, valueTotalLength);
  // All size fields stored as uint32_t for cross-platform compatibility.
  uint32_t numItems = static_cast<uint32_t>(lexicon->Length());
  fwrite(&numItems, sizeof(uint32_t), 1, fp);

  uint32_t u32KeyTotal = static_cast<uint32_t>(keyTotalLength);
  fwrite(&u32KeyTotal, sizeof(uint32_t), 1, fp);
  fwrite(keyBuf.c_str(), sizeof(char), keyTotalLength, fp);
  uint32_t u32ValueTotal = static_cast<uint32_t>(valueTotalLength);
  fwrite(&u32ValueTotal, sizeof(uint32_t), 1, fp);
  fwrite(valueBuf.c_str(), sizeof(char), valueTotalLength, fp);

  size_t keyCursor = 0, valueCursor = 0;
  for (const DictEntry* entry : *lexicon) {
    uint32_t numValues = static_cast<uint32_t>(entry->NumValues());
    fwrite(&numValues, sizeof(uint32_t), 1, fp);
    uint32_t keyOffset = static_cast<uint32_t>(keyOffsets[keyCursor++]);
    fwrite(&keyOffset, sizeof(uint32_t), 1, fp);
    for (size_t i = 0; i < numValues; i++) {
      uint32_t valueOffset = static_cast<uint32_t>(valueOffsets[valueCursor++]);
      fwrite(&valueOffset, sizeof(uint32_t), 1, fp);
    }
  }
  assert(keyCursor == numItems);
}

BinaryDictPtr BinaryDict::NewFromFile(FILE* fp) {
  BinaryDictPtr dict(new BinaryDict(LexiconPtr(new Lexicon)));

  // Read uint32 fields; the .ocd files were generated on a 32-bit platform
  // where sizeof(size_t) == 4, so on 64-bit we must read fixed 4-byte values.

  uint32_t numItems32;
  size_t unitsRead = fread(&numItems32, sizeof(uint32_t), 1, fp);
  if (unitsRead != 1) {
    throw InvalidFormat("Invalid OpenCC binary dictionary (numItems)");
  }
  size_t numItems = numItems32;

  uint32_t keyTotalLength32;
  unitsRead = fread(&keyTotalLength32, sizeof(uint32_t), 1, fp);
  if (unitsRead != 1) {
    throw InvalidFormat("Invalid OpenCC binary dictionary (keyTotalLength)");
  }
  size_t keyTotalLength = keyTotalLength32;
  dict->keyBuffer.resize(keyTotalLength);
  unitsRead = fread(const_cast<char*>(dict->keyBuffer.c_str()), sizeof(char),
                    keyTotalLength, fp);
  if (unitsRead != keyTotalLength) {
    throw InvalidFormat("Invalid OpenCC binary dictionary (keyBuffer)");
  }

  uint32_t valueTotalLength32;
  unitsRead = fread(&valueTotalLength32, sizeof(uint32_t), 1, fp);
  if (unitsRead != 1) {
    throw InvalidFormat("Invalid OpenCC binary dictionary (valueTotalLength)");
  }
  size_t valueTotalLength = valueTotalLength32;
  dict->valueBuffer.resize(valueTotalLength);
  unitsRead = fread(const_cast<char*>(dict->valueBuffer.c_str()), sizeof(char),
                    valueTotalLength, fp);
  if (unitsRead != valueTotalLength) {
    throw InvalidFormat("Invalid OpenCC binary dictionary (valueBuffer)");
  }

  for (size_t i = 0; i < numItems; i++) {
    uint32_t numValues32;
    unitsRead = fread(&numValues32, sizeof(uint32_t), 1, fp);
    if (unitsRead != 1) {
      throw InvalidFormat("Invalid OpenCC binary dictionary (numValues)");
    }
    size_t numValues = numValues32;

    uint32_t keyOffset32;
    unitsRead = fread(&keyOffset32, sizeof(uint32_t), 1, fp);
    if (unitsRead != 1) {
      throw InvalidFormat("Invalid OpenCC binary dictionary (keyOffset)");
    }
    const char* key = dict->keyBuffer.c_str() + keyOffset32;

    vector<const char*> values;
    for (size_t j = 0; j < numValues; j++) {
      uint32_t valueOffset32;
      unitsRead = fread(&valueOffset32, sizeof(uint32_t), 1, fp);
      if (unitsRead != 1) {
        throw InvalidFormat("Invalid OpenCC binary dictionary (valueOffset)");
      }
      const char* value = dict->valueBuffer.c_str() + valueOffset32;
      values.push_back(value);
    }
    PtrDictEntry* entry = new PtrDictEntry(key, values);
    dict->lexicon->Add(entry);
  }

  return dict;
}

void BinaryDict::ConstructBuffer(string& keyBuf, vector<size_t>& keyOffset,
                                 size_t& keyTotalLength, string& valueBuf,
                                 vector<size_t>& valueOffset,
                                 size_t& valueTotalLength) const {
  keyTotalLength = 0;
  valueTotalLength = 0;
  // Calculate total length
  for (const DictEntry* entry : *lexicon) {
    keyTotalLength += entry->KeyLength() + 1;
    assert(entry->NumValues() != 0);
    if (entry->NumValues() == 1) {
      const auto* svEntry = static_cast<const SingleValueDictEntry*>(entry);
      valueTotalLength += strlen(svEntry->Value()) + 1;
    } else {
      const auto* mvEntry = static_cast<const MultiValueDictEntry*>(entry);
      for (const auto& value : mvEntry->Values()) {
        valueTotalLength += strlen(value) + 1;
      }
    }
  }
  // Write keys and values to buffers
  keyBuf.resize(keyTotalLength, '\0');
  valueBuf.resize(valueTotalLength, '\0');
  char* pKeyBuffer = const_cast<char*>(keyBuf.c_str());
  char* pValueBuffer = const_cast<char*>(valueBuf.c_str());
  for (const DictEntry* entry : *lexicon) {
    strcpy(pKeyBuffer, entry->Key());
    keyOffset.push_back(pKeyBuffer - keyBuf.c_str());
    pKeyBuffer += entry->KeyLength() + 1;
    if (entry->NumValues() == 1) {
      const auto* svEntry = static_cast<const SingleValueDictEntry*>(entry);
      strcpy(pValueBuffer, svEntry->Value());
      valueOffset.push_back(pValueBuffer - valueBuf.c_str());
      pValueBuffer += strlen(svEntry->Value()) + 1;
    } else {
      const auto* mvEntry = static_cast<const MultiValueDictEntry*>(entry);
      for (const auto& value : mvEntry->Values()) {
        strcpy(pValueBuffer, value);
        valueOffset.push_back(pValueBuffer - valueBuf.c_str());
        pValueBuffer += strlen(value) + 1;
      }
    }
  }
  assert(keyBuf.c_str() + keyTotalLength == pKeyBuffer);
  assert(valueBuf.c_str() + valueTotalLength == pValueBuffer);
}

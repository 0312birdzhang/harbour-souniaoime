Name:       harbour-souniaoime

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}

Version:    0.3.9
Release:    1
Summary:    PinyinIme for Sailfish OS
License:    LGPLv2
Group:      Qt/Qt
Source:     %{name}-%{version}.tar.gz
URL:        https://github.com/0312birdzhang/harbour-soumaoime
Requires:   jolla-keyboard
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  python3-base
BuildRequires:  desktop-file-utils
BuildRequires:  doxygen
BuildRequires:  cmake

%description
Allows you to use google pinyin to enter Simple Chinese character on Sailfish OS.


%prep
%setup -q -n %{name}-%{version}
sed -i \
    -e 's:BIN python:BIN /usr/bin/python3:g' \
    libopencc/data/CMakeLists.txt

%build
# do nothing
cd libopencc && %cmake \
  -DCMAKE_SKIP_RPATH=OFF
%qtc_qmake5

%qtc_make %{?_smp_mflags}



%install
rm -rf %{buildroot}
%qmake5_install



# << install pre

# >> install post
# << install post

%post
# >> post
systemctl-user restart maliit-server || true

# << post

%postun
# >> postun
systemctl-user restart maliit-server || true
# << postun

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%{_bindir}
%{_datadir}/maliit
%{_datadir}/%{name}
%{_datadir}/%{name}/qml
%{_datadir}/%{name}/data
%{_datadir}/icons/
%{_datadir}/applications/%{name}.desktop
%{_libdir}/qt5/qml/xyz/birdzhang/ime
%{_libdir}/qt5/qml/xyz/birdzhang/opencc

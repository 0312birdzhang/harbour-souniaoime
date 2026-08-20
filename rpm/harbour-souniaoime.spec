Name:       harbour-souniaoime

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}

%define __requires_exclude ^lib(xyz.birdzhang.ime|xyz.birdzhang.wubi|xyz.birdzhang.stroke).*$
%global __provides_exclude_from %{_libdir}/qt5/qml/.*

Version:    0.4.3
Release:    1
Summary:    PinyinIme for Sailfish OS
License:    LGPLv2
Group:      Qt/Qt
Source:     %{name}-%{version}.tar.gz
URL:        https://github.com/0312birdzhang/harbour-soumaoime
Requires:   jolla-keyboard >= 0.9.10
BuildRequires:  pkgconfig(sailfishapp)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  desktop-file-utils >= 0.26
BuildRequires:  doxygen >= 1.9.5
BuildRequires:  cmake >= 3.27.1
BuildRequires:  python3-base >= 3.8.18

%description
Allows you to use google pinyin and stroke to enter Simple Chinese character on Sailfish OS.


%prep
%setup -q -n %{name}-%{version}

%build

%qtc_qmake5

%qtc_make %{?_smp_mflags}



%install
rm -rf %{buildroot}
%qmake5_install

mkdir -p %{buildroot}/usr/share/maliit/plugins/com/jolla/handlers
cd %{buildroot}/usr/share/maliit/plugins/com/jolla/handlers \
  && cp ../SouniaoPinyinHandler.qml SouniaoPinyinHandler.qml \
  && cp ../SouniaoStrokeHandler.qml SouniaoStrokeHandler.qml \
  && cp ../SouniaoWubiHandler.qml SouniaoWubiHandler.qml

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
%{_bindir}/harbour-souniaoime
%{_datadir}/maliit/plugins/com/jolla/SouniaoPinyinHandler.qml
%{_datadir}/maliit/plugins/com/jolla/SouniaoStrokeHandler.qml
%{_datadir}/maliit/plugins/com/jolla/SouniaoWubiHandler.qml
%{_datadir}/maliit/plugins/com/jolla/handlers/SouniaoPinyinHandler.qml
%{_datadir}/maliit/plugins/com/jolla/handlers/SouniaoStrokeHandler.qml
%{_datadir}/maliit/plugins/com/jolla/handlers/SouniaoWubiHandler.qml
%{_datadir}/maliit/plugins/com/jolla/layouts/layouts_souniaoime.conf
%{_datadir}/maliit/plugins/com/jolla/layouts/zh_cn_souniaoime.qml
%{_datadir}/maliit/plugins/com/jolla/layouts/zh_cn_souniaostroke.qml
%{_datadir}/maliit/plugins/com/jolla/layouts/zh_cn_souniaowubi.qml
%{_datadir}/%{name}
%{_datadir}/icons/hicolor/86x86/apps/harbour-souniaoime.png
%{_datadir}/applications/%{name}.desktop
%{_libdir}/qt5/qml/xyz/birdzhang/ime
%{_libdir}/qt5/qml/xyz/birdzhang/stroke
%{_libdir}/qt5/qml/xyz/birdzhang/wubi

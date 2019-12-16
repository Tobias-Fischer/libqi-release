%global __os_install_post %(echo '%{__os_install_post}' | sed -e 's!/usr/lib[^[:space:]]*/brp-python-bytecompile[[:space:]].*$!!g')
%global __provides_exclude_from ^/opt/ros/melodic/.*$
%global __requires_exclude_from ^/opt/ros/melodic/.*$

Name:           ros-melodic-naoqi-libqi
Version:        2.9.0
Release:        5%{?dist}
Summary:        ROS naoqi_libqi package

License:        BSD
URL:            http://doc.aldebaran.com/libqi
Source0:        %{name}-%{version}.tar.gz

Requires:       boost-devel
Requires:       boost-python2-devel
Requires:       boost-python3-devel
BuildRequires:  boost-devel
BuildRequires:  boost-python2-devel
BuildRequires:  boost-python3-devel
BuildRequires:  ros-melodic-catkin

%description
Aldebaran's libqi: a core library for NAOqiOS development

%prep
%autosetup

%build
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree that was dropped by catkin, and source it.  It will
# set things like CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/melodic/setup.sh" ]; then . "/opt/ros/melodic/setup.sh"; fi
mkdir -p obj-%{_target_platform} && cd obj-%{_target_platform}
%cmake3 \
    -UINCLUDE_INSTALL_DIR \
    -ULIB_INSTALL_DIR \
    -USYSCONF_INSTALL_DIR \
    -USHARE_INSTALL_PREFIX \
    -ULIB_SUFFIX \
    -DCMAKE_INSTALL_LIBDIR="lib" \
    -DCMAKE_INSTALL_PREFIX="/opt/ros/melodic" \
    -DCMAKE_PREFIX_PATH="/opt/ros/melodic" \
    -DSETUPTOOLS_DEB_LAYOUT=OFF \
    -DCATKIN_BUILD_BINARY_PACKAGE="1" \
    ..

%make_build

%install
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree that was dropped by catkin, and source it.  It will
# set things like CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/melodic/setup.sh" ]; then . "/opt/ros/melodic/setup.sh"; fi
%make_install -C obj-%{_target_platform}

%files
/opt/ros/melodic

%changelog
* Mon Dec 16 2019 Surya Ambrose <surya.ambrose@gmail.com> - 2.9.0-5
- Autogenerated by Bloom

* Mon Dec 16 2019 Surya Ambrose <surya.ambrose@gmail.com> - 2.9.0-4
- Autogenerated by Bloom

* Mon Dec 16 2019 Surya Ambrose <surya.ambrose@gmail.com> - 2.9.0-3
- Autogenerated by Bloom

* Mon Dec 16 2019 Surya Ambrose <surya.ambrose@gmail.com> - 2.9.0-2
- Autogenerated by Bloom

* Mon Dec 16 2019 Surya Ambrose <surya.ambrose@gmail.com> - 2.9.0-1
- Autogenerated by Bloom


#!/bin/bash

rm hello.c

echo '#include <stdio.h>'			>> hello.c
echo 'int main()'					>> hello.c
echo '{'							>> hello.c
echo '	printf("hello world!\n");'	>> hello.c
echo '	return 0;'					>> hello.c
echo '}'							>> hello.c



clang hello.c -o hello-armv7 -arch armv7 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS11.2.sdk -I /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS11.2.sdk/usr/include
file hello-armv7

clang hello.c -o hello-armv7s -arch armv7s -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS11.2.sdk -I /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS11.2.sdk/usr/include
file hello-armv7s

clang hello.c -o hello-arm64 -arch arm64 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS11.2.sdk -I /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS11.2.sdk/usr/include
file hello-arm64

clang hello.c -o hello-i386 -arch i386
file hello-i386

clang hello.c -o hello-x86_64 -arch x86_64
file hello-x86_64



lipo hello-armv7 hello-armv7s hello-arm64 hello-i386 hello-x86_64 -create -output hello_5_arches

lipo hello-armv7 hello-armv7s hello-arm64 hello-i386 -create -output hello_4_arches


lipo MachO-iOS-arm1176JZFS-bash MachO-OSX-ppc-openssl-1.0.1h hello-armv7 hello-armv7s hello-arm64 hello-i386 -create -output hello_6_arches

lipo MachO-iOS-arm1176JZFS-bash MachO-OSX-ppc-openssl-1.0.1h hello-armv7 hello-armv7s hello-arm64 hello-i386 hello-x86_64 -create -output hello_7_arches

file hello_7_arches

#/usr/bin/nm hello_4_arches | less

#/usr/bin/nm hello_5_arches | less



#rm -rf hello-armv7 hello-armv7s hello-arm64 hello-i386 hello-x86_64 hello_4_arches hello_5_arches

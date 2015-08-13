#!/usr/bin/perl
# 
# Copyright (c) 2013 No Face Press, LLC
# License http://opensource.org/licenses/mit-license.php MIT License
#

# This script builds and packages a Windows release.
# It requires ActiveState Perl to use and is intended
# to be run from the its directory under the 
# VS Developer Command Prompt.

# Create a Zip file
use Archive::Zip qw( :ERROR_CODES :CONSTANTS );
my $zip = Archive::Zip->new();

my $src = "..";

sub getCivetwebVersion {
    print "Fetching CivetWeb version...\n";
    open HEADER, "${src}/include/civetweb.h";
    while (<HEADER>) {
        if (m/define\s+CIVETWEB_VERSION\s+"(.+)"/) {
            close HEADER;
            return $1;
        }
    }
    close HEADER;
    return "UNKNOWN_VERSION";
}

my $CIVETWEB_VERSION = getCivetwebVersion();
my $basename         = "civetweb-$CIVETWEB_VERSION";
my $dir              = "${basename}";

sub build32() {
    print "\nBuilding Win32 Release version...\n";
    system("msbuild /p:Configuration=Release /p:Platform=Win32 civetweb.sln");
}

sub build64() {
    print "\nBuilding x64 Release version...\n";
    system("msbuild /p:Configuration=Release /p:Platform=x64 civetweb.sln");
}

sub writeArchive() {
    my $archive = "${basename}-win.zip";
    print "Creating archive $archive ...\n";

    $zip->addDirectory("${dir}/");

    $zip->addFile( "${src}/LICENSE.md",            "${dir}/LICENSE.md" );
    $zip->addFile( "${src}/README.md",             "${dir}/README.md" );
    $zip->addFile( "${src}/resources/systray.ico", "${dir}/systray.ico" );
    $zip->addFile( "${src}/resources/civetweb_64x64.png",
        "${dir}/civetweb_64x64.png" );
    $zip->addFile( "${src}/resources/itworks.html", "${dir}/index.html" );
    $zip->addFile( "${src}/VS2012/Release/Win32/civetweb_lua.exe",
        "${dir}/civetweb32.exe" );
    $zip->addFile( "${src}/VS2012/Release/x64/civetweb_lua.exe",
        "${dir}/civetweb64.exe" );

    unless ( $zip->writeToFileNamed($archive) == AZ_OK ) {
        die 'write error';
    }

}

build32();
build64();
writeArchive();
exit 0;

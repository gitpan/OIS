use ExtUtils::MakeMaker;
use strict;
use warnings;

main();
exit 0;

sub main {
    my $fixed_args = fixed_args('OIS');
    my $varied_args = varied_args();

    WriteMakefile(%$fixed_args, %$varied_args);
}

sub fixed_args {
    my ($pkg) = @_;

    return {
        'NAME'          => $pkg,
        'VERSION_FROM'  => "$pkg.pm",
        'ABSTRACT_FROM' => "$pkg.pm",
        'AUTHOR'        => 'Scott Lanning <slanning@cpan.org>',
        'CC'            => $ENV{'CXX'} || 'g++',
        'LD'            => '$(CC)',
        'OBJECT'        => '$(O_FILES)',
        'XSOPT'         => '-C++',
        'TYPEMAPS'      => ['perlobject.map'],
    };
}

sub varied_args {
    # Make sure libs are known by pkg-config
    my @errors = ();
    push @errors, check_pkg_config('OIS', '1.0.0');
    if (@errors) {
        die(map { "$_$/" } @errors);
    }

    # Get include dirs and defines
    my @cflags = ();
    push @cflags, pkg_config('OIS', 'cflags');

    # Get lib dirs
    my @libs = ();
    push @libs, pkg_config('OIS', 'libs');

    return {
        'PREREQ_PM'         => {
            'Scalar::Util' => 1.18,   # bugfix for looks_like_number
            'Test::More' => 0,
        },
        'INC'               => join(' ', @cflags, '-I.'),
        'LIBS'              => join(' ', @libs),
    };
}

sub check_pkg_config {
    my ($pkg, $required_version) = @_;

    my $pc = 'pkg-config';
    my @errors = ();

    # Check that pkg-config is installed
    my $pcver = `$pc --version`;
    if ($pcver eq '') {
        push @errors, "$pc not found";
    }
    else {
        # Check that pkg-config knows about the package
        my $pkgexists = `$pc --exists $pkg`;
        if ($?) {
            push @errors, "Package $pkg not found by $pc";
        }
        else {
            # Check that the package is the right version
            my $pkgver = `$pc --atleast-version='$required_version' $pkg`;
            if ($?) {
                push @errors, "Package $pkg is not the right version (at least $required_version)";
            }
        }
    }

    return @errors;
}

sub pkg_config {
    my ($pkg, $option) = @_;

    my $pc = 'pkg-config';

    my $str = `$pc --$option $pkg`;
    if ($?) {
        die "$pc --$option $pkg: $str\n";
    }
    else {
        chomp $str;
        return $str;
    }
}

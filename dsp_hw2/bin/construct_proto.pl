#!/usr/bin/perl

# this perl script can help you construct HMM prototype

use strict;
use warnings;

sub usage
{
	my $msg = "this script will output mmf proto,\n".
	"usage: $0 <hcopy.cfg> <state num>\n".
	"\thcopy.cfg must contant information of NUMCEPS and TARGETKIND\n";
	print STDERR $msg;
	exit 1;
}

&usage() if( scalar @ARGV != 2 ); 

my ($hcopy_cfg, $state_num ) = @ARGV;
my $out_name = "proto";

sub loadConfig
{
	my $config_file = shift;
	my %config = ();
	open HCOPY_CFG,"<$config_file" or die "'$config_file' $!";
	while( <HCOPY_CFG> ){
		next if( /^\s*#/ );

		chomp;
		my @token = split(/=/);
		$config{ $token[0] } = $token[1];
	}
	return %config;
}

my %config = &loadConfig( $hcopy_cfg );

die "no mfcc NUMCEPS config\n" unless exists $config{'NUMCEPS'};
die "no mfcc TARGETKIND config\n" unless exists $config{'TARGETKIND'};

my ($base_dim, $feature_dim);
$base_dim = $config{'NUMCEPS'};
$base_dim += 1 if( $config{'TARGETKIND'} =~ /_E/ );

$feature_dim = $base_dim;
$feature_dim += $base_dim if( $config{'TARGETKIND'} =~ /_D/ );
$feature_dim += $base_dim if( $config{'TARGETKIND'} =~ /_A/ );

open OUT,">$out_name" or die "open error $out_name $!";

### dump header information ###
print OUT "~o <VECSIZE> $feature_dim <$config{'TARGETKIND'}>\n";

print OUT "~h \"$out_name\"\n";
print OUT '<BeginHMM>'."\n";
print OUT "<NumStates> $state_num\n";

for my $i ( 2 .. $state_num-1 ){
	print OUT "<State> $i\n";
	print OUT "<Mean> $feature_dim\n";
	print OUT "0.0 " for( 1 .. $feature_dim-1 );
	print OUT "0.0\n";
	print OUT "<Variance> $feature_dim\n";
	print OUT "1.0 " for( 1 .. $feature_dim-1 );
	print OUT "1.0\n";
}

print OUT "<TransP> $state_num\n";

print OUT "0.0 1.0";
print OUT " 0.0" for ( 3 .. $state_num );
print OUT "\n";

for my $i ( 2 .. $state_num-1 ){
	for my $j ( 1 .. $state_num ){
		if( $i +1 == $j or $i == $j ){
			print OUT "0.5 ";
		}else{
			print OUT "0.0 ";
		}
	}
	print OUT "\n";
}

print OUT "0.0 " for ( 1 .. $state_num-1 );
print OUT "0.0\n";

print OUT "<EndHMM>\n";

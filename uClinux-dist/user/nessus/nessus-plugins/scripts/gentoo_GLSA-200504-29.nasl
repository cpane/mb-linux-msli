# This script was automatically generated from 
#  http://www.gentoo.org/security/en/glsa/glsa-200504-29.xml
# It is released under the Nessus Script Licence.
# The messages are release under the Creative Commons - Attribution /
# Share Alike license. See http://creativecommons.org/licenses/by-sa/2.0/
#
# Avisory is copyright 2001-2005 Gentoo Foundation, Inc.
# GLSA2nasl Convertor is copyright 2004 Michel Arboi <mikhail@nessus.org>

if (! defined_func('bn_random')) exit(0);

if (description)
{
 script_id(18168);
 script_version("$Revision: 1.2 $");
 script_xref(name: "GLSA", value: "200504-29");

 desc = 'The remote host is affected by the vulnerability described in GLSA-200504-29
(Pound: Buffer overflow vulnerability)


    Steven Van Acker has discovered a buffer overflow vulnerability in
    the "add_port()" function in Pound.
  
Impact

    A remote attacker could send a request for an overly long hostname
    parameter, which could lead to the remote execution of arbitrary code
    with the rights of the Pound daemon process (by default, Gentoo uses
    the "nobody" user to run the Pound daemon).
  
Workaround

    There is no known workaround at this time.
  
References:
    http://www.apsis.ch/pound/pound_list/archive/2005/2005-04/1114516112000


Solution: 
    All Pound users should upgrade to the latest version:
    # emerge --sync
    # emerge --ask --oneshot --verbose ">=www-servers/pound-1.8.3"
  

Risk factor : High
';
 script_description(english: desc);
 script_copyright(english: "(C) 2005 Michel Arboi <mikhail@nessus.org>");
 script_name(english: "[GLSA-200504-29] Pound: Buffer overflow vulnerability");
 script_category(ACT_GATHER_INFO);
 script_family(english: "Gentoo Local Security Checks");
 script_dependencies("ssh_get_info.nasl");
 script_require_keys('Host/Gentoo/qpkg-list');
 script_summary(english: 'Pound: Buffer overflow vulnerability');
 exit(0);
}

include('qpkg.inc');
if (qpkg_check(package: "www-servers/pound", unaffected: make_list("ge 1.8.3"), vulnerable: make_list("lt 1.8.3")
)) { security_hole(0); exit(0); }

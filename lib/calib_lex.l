%option noyywrap

%{

/*
    lib/calib_lex.l
    code for parsing calibration file, generated by flex

    Copyright (C) 2003 Frank Mori Hess <fmhess@users.sourceforge.net

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation, version 2.1
    of the License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
    USA.
*/

#include <string.h>
#include "libinternal.h"
#include "calib_yacc.h"

%}

%x COMMENT
%x STRING

%%

%{
	calib_llocp->first_line = 1;
%}

<STRING,INITIAL>\n { calib_llocp->first_line++; }

"#" { BEGIN(COMMENT); }
<COMMENT>\n { calib_llocp->first_line++; BEGIN(INITIAL); }
<COMMENT>.

\" { BEGIN(STRING); }
<STRING>[^\"]*\" {
	if( strlen( calib_yytext ) > 0 )
		calib_yytext[ strlen( calib_yytext ) - 1 ] = 0;
	calib_lvalp->sval = calib_yytext;
	BEGIN(INITIAL);
	return ( T_STRING );
}

driver_name	{ return ( T_DRIVER_NAME ); }
board_name	{ return ( T_BOARD_NAME ); }
calibrations	{ return ( T_CALIBRATIONS ); }
subdevice	{ return ( T_SUBDEVICE); }
channels	{ return (T_CHANNELS); }
ranges	{ return ( T_RANGES ); }
arefs	{ return ( T_AREFS ); }
caldacs	{ return ( T_CALDACS ); }
channel	{ return ( T_CHANNEL ); }
value	{ return ( T_VALUE ); }
=>	{ return ( T_ASSIGN ); };

(0[xX])?(00)?[0-9a-fA-F]+ { calib_lvalp->ival = strtol( calib_yytext, NULL, 0 );
	return( T_NUMBER ); }

[ \t]

.	{ return( calib_yytext[0] ); }


%%

/* ########################################################################

   PICSimLab - Programmable IC Simulator Laboratory

   ########################################################################

   Copyright (c) : 2010-2024  Luis Claudio Gambôa Lopes <lcgamboa@yahoo.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */

#ifndef TYPES_H
#define TYPES_H

enum PICSimlabCPUState { CPU_RUNNING, CPU_STEPPING, CPU_HALTED, CPU_BREAKPOINT, CPU_ERROR, CPU_POWER_OFF, CPU_LAST };

enum PICSimlabStatus { PS_RUN = 0, PS_DEBUG, PS_SERIAL, PS_LAST };

enum PICSimlabGUIType { GT_GAUGE = 0, GT_SCROLL, GT_LABEL, GT_COMBO, GT_BUTTON, GT_LAST };
enum PICSimlabGUIAction { GA_ADD = 0, GA_DEL, GA_SET, GA_GET, GA_SET_LABEL, GA_LAST };

enum PICSimlabGUIMenu { GMT_HEX, GMT_BIN, GMT_DISABLED, GMT_LAST };

enum PICSimLabWindowAction {
    PWA_COMBOSETITEMS,
    PWA_COMBOSETTEXT,
    PWA_COMBOGETTEXT,
    PWA_COMBOPROPEV,
    PWA_COMBOPARTEV,

    PWA_SPINDSETMAX,
    PWA_SPINDSETMIN,
    PWA_SPINDSETVALUE,
    PWA_SPINDGETVALUE,

    PWA_SPINSETMAX,
    PWA_SPINSETMIN,
    PWA_SPINSETVALUE,
    PWA_SPINGETVALUE,
    PWA_SPINPROPEV,

    PWA_TEXTCLEAR,
    PWA_TEXTADDLINE,
    PWA_TEXTGETLINE,
    PWA_TEXTGETLINECOUNT,
    PWA_TEXTAPPEND,
    PWA_TEXTTELETYPE,

    PWA_EDITSETTEXT,
    PWA_EDITGETTEXT,
    PWA_EDITPARTEV,

    PWA_LABELSETTEXT,

    PWA_CHECKBOXSETCHECK,
    PWA_CHECKBOXGETCHECK,

    PWA_SETENABLE,
    PWA_SETWIDTH,
    PWA_SETHEIGHT,
    PWA_SETVISIBLE,
    PWA_SETX,
    PWA_SETY,
    PWA_SETTAG,
    PWA_GETWIDTH,
    PWA_GETHEIGHT,
    PWA_GETX,
    PWA_GETY,
    PWA_GETTAG,

    PWA_WINDOWCREATE,
    PWA_WINDOWDESTROY,
    PWA_WINDOWLOADXML,
    PWA_WINDOWSHOW,
    PWA_WINDOWHIDE,
    PWA_WINDOWPARTEV,
    PWA_WINDOWSHOWEX,
    PWA_WINDOWHIDEEX,

    PWA_BUTTONPARTEV,
    PWA_BUTTONBOARDEV,

    PWA_LAST,
};

#endif  // TYPES_H
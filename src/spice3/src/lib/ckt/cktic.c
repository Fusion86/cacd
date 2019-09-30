/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/

#include "spice.h"
#include <stdio.h>
#include "cktdefs.h"
#include "smpdefs.h"
#include "util.h"
#include "sperror.h"
#include "devdefs.h"

extern SPICEdev *DEVices[];

int CKTic(CKTcircuit *ckt)
{
    int error;
    int size;
    register int i;
    register CKTnode *node;

    size = SMPmatSize(ckt->CKTmatrix);
    for (i = 0; i <= size; i++) {
        ckt->CKTrhs[i] = 0;
    }

    for (node = ckt->CKTnodes; node; node = node->next) {
        if (node->nsGiven) {
            node->ptr = SMPmakeElt(ckt->CKTmatrix, node->number, node->number);
            if (!node->ptr) return(E_NOMEM);
            ckt->CKThadNodeset = 1;
            ckt->CKTrhs[node->number] = node->nodeset;
        }
        if (node->icGiven) {
            if (!node->ptr) {
                node->ptr = SMPmakeElt(ckt->CKTmatrix, node->number, node->number);
                if (!node->ptr) return(E_NOMEM);
            }
            ckt->CKTrhs[node->number] = node->ic;
        }
    }

    if (ckt->CKTmode & MODEUIC) {
        for (i = 0; i < DEVmaxnum; i++) {
            if (DEVices[i]->DEVsetic && ckt->CKThead[i]) {
                error = (*(DEVices[i]->DEVsetic))(ckt->CKThead[i], ckt);
                if (error) return(error);
            }
        }
    }

    return(OK);
}

/*
* kinetic-c
* Copyright (C) 2014 Seagate Technology.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
*/

#ifndef _KINETIC_PDU_H
#define _KINETIC_PDU_H

#include "kinetic_types_internal.h"

void KineticPDU_Init(KineticPDU* const pdu, KineticConnection* const connection);
KineticStatus KineticPDU_Send(KineticPDU* request);
KineticStatus KineticPDU_ReceiveMain(KineticPDU* response);
KineticStatus KineticPDU_ReceiveValue(int socket_desc, ByteBuffer* value, size_t value_length);
size_t KineticPDU_GetValueLength(KineticPDU* const pdu);
KineticStatus KineticPDU_GetStatus(KineticPDU* pdu);
KineticProto_Command_KeyValue* KineticPDU_GetKeyValue(KineticPDU* pdu);
KineticProto_Command_Range* KineticPDU_GetKeyRange(KineticPDU* pdu);

#endif // _KINETIC_PDU_H

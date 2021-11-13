/**
 *
 *  Copyright (C) 2020  Raul Casanova Marques
 *
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __PROTOCOL_CONTROLLER_MULTOS_USER_H_
#define __PROTOCOL_CONTROLLER_MULTOS_USER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NDEBUG
# include <stdio.h>
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <mcl/bn_c256.h>
#include <openssl/sha.h>

#include "config/config.h"

#include "models/user.h"
#include "system.h"

#include "multos/apdu.h"
#include "apdu/command.h"
#include "pcsc/reader.h"

#include "helpers/mcl_helper.h"
#include "helpers/multos_helper.h"

/**
 * Gets the user identifier using the specified reader.
 *
 * @param reader the reader to be used
 * @param identifier the user identifier
 * @return 0 if success else -1
 */
extern int ue_get_user_identifier(reader_t reader, user_identifier_t *identifier);

#ifdef __cplusplus
}
#endif

#endif /* __PROTOCOL_CONTROLLER_MULTOS_USER_H_ */

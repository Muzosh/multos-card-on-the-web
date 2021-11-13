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

#ifndef __PROTOCOL_SETUP_H_
#define __PROTOCOL_SETUP_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>

#include <mcl/bn_c256.h>

#include "system.h"

/**
 * Outputs the system parameters.
 *
 * @param parameters the system parameters
 * @return 0 if success else -1
 */
extern int sys_setup(system_par_t *parameters);

#ifdef __cplusplus
}
#endif

#endif /* __PROTOCOL_SETUP_H_ */

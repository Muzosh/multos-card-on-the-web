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

#ifndef __CONFIG_H_
#define __CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define APDU_L_MAX  256

/*
 * Length of the SHA1 hash
 */
#define SHA_DIGEST_LENGTH 20

/*
 * Padding of the SHA1 hash
 */
#define SHA_DIGEST_PADDING 12

/*
 * Maximum length of user id
 */
#define USER_MAX_ID_LENGTH 21

#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H_ */

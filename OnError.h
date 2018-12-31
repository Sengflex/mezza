/*
 * OnError.h
 *
 *  Created on: 8 de nov de 2018
 *      Author: danilo
 */

#ifndef onerror_H_
#define onerror_H_

#define onerror(COND) if(!(COND))

#define SHOWERROR(FPATH, MSG) \
            fprintf(stderr, "\nERROR: %s: %s\n", FPATH, MSG);

#define SHOWLOCAL(FILE, LINE) fprintf(stderr, "\tLocal: %s(%d)\n", FILE, LINE);

#define PROTECT(COND, FPATH, MSG, AFTER) onerror(COND) { \
            SHOWERROR(FPATH, MSG) \
            SHOWLOCAL(__FILE__, __LINE__) \
            AFTER \
        }\

#endif /* onerror_H_ */

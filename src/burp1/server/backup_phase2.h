#ifndef _BURP1_BACKUP_PHASE2_SERVER_H
#define _BURP1_BACKUP_PHASE2_SERVER_H

extern int backup_phase2_server(struct asfd *asfd,
	struct sdirs *sdirs, struct conf *cconf,
	gzFile *cmanfp, struct dpthl *dpthl, int resume);

#endif

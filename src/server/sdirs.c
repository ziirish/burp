#include "include.h"

struct sdirs *sdirs_alloc(void)
{
        struct sdirs *sdirs;
        if((sdirs=(struct sdirs *)calloc(1, sizeof(struct sdirs))))
		return sdirs;
	log_out_of_memory(__FUNCTION__);
	return NULL;
}

int sdirs_init(struct sdirs *sdirs, struct config *conf, const char *client)
{
	int ret=0;
	char *lockbase=NULL;

	if(!conf->directory)
	{
		logp("conf->directory unset in %s\n", __FUNCTION__);
		goto error;
	}
	if(!conf->dedup_group)
	{
		logp("conf->dedup_group unset in %s\n", __FUNCTION__);
		goto error;
	}

	if(!(sdirs->base=strdup(conf->directory))
	  || !(sdirs->dedup=prepend_s(sdirs->base, conf->dedup_group))
	  || !(sdirs->data=prepend_s(sdirs->dedup, "data"))
	  || !(sdirs->clients=prepend_s(sdirs->dedup, "clients"))
	  || !(sdirs->client=prepend_s(sdirs->clients, client)))
		goto error;

	if(!(sdirs->working=prepend_s(sdirs->client, "working"))
	  || !(sdirs->finishing=prepend_s(sdirs->client, "finishing"))
	  || !(sdirs->current=prepend_s(sdirs->client, "current")))
		goto error;

	if(!(sdirs->timestamp=prepend_s(sdirs->working, "timestamp"))
	  || !(sdirs->changed=prepend_s(sdirs->working, "changed"))
	  || !(sdirs->unchanged=prepend_s(sdirs->working, "unchanged"))
	  || !(sdirs->cmanifest=prepend_s(sdirs->current, "manifest")))
		goto error;

	if(conf->client_lockdir)
	{
		if(!(sdirs->lock=strdup(conf->client_lockdir))
		  || !(lockbase=prepend_s(sdirs->lock, client)))
			goto error;
	}
	else
	{
		if(!(sdirs->lock=strdup(sdirs->client))
		  || !(lockbase=strdup(sdirs->client)))
			goto error;
	}
	if(!(sdirs->lockfile=prepend_s(lockbase, "lockfile")))
		goto error;

	goto end;
error:
	sdirs_free(sdirs);
	ret=-1;
end:
	if(lockbase) free(lockbase);
	return ret;
}

void sdirs_free(struct sdirs *sdirs)
{
	if(!sdirs) return;

        if(sdirs->base) free(sdirs->base);
        if(sdirs->dedup) free(sdirs->dedup);
        if(sdirs->data) free(sdirs->data);
        if(sdirs->clients) free(sdirs->clients);
        if(sdirs->client) free(sdirs->client);

        if(sdirs->working) free(sdirs->working);
        if(sdirs->finishing) free(sdirs->finishing);
        if(sdirs->current) free(sdirs->current);

        if(sdirs->timestamp) free(sdirs->timestamp);
        if(sdirs->changed) free(sdirs->changed);
        if(sdirs->unchanged) free(sdirs->unchanged);
        if(sdirs->cmanifest) free(sdirs->cmanifest);

        if(sdirs->lock) free(sdirs->lock);
        if(sdirs->lockfile) free(sdirs->lockfile);

	free(sdirs);
	sdirs=NULL;
}

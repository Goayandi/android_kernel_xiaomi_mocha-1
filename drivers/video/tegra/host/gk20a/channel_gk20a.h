/*
 * drivers/video/tegra/host/gk20a/channel_gk20a.h
 *
 * GK20A graphics channel
 *
 * Copyright (c) 2011, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */
#ifndef __CHANNEL_GK20A_H__
#define __CHANNEL_GK20A_H__

#include <linux/log2.h>
#include <linux/slab.h>
#include <linux/wait.h>

struct gk20a;
struct gr_gk20a;
struct mem_mgr;
struct mem_handle;

#include "nvhost_channel.h"
#include "nvhost_hwctx.h"

#include "cdma_gk20a.h"
#include "mm_gk20a.h"
#include "gr_gk20a.h"

struct gpfifo {
	u32 entry0;
	u32 entry1;
};

struct notification {
	struct {
		u32 nanoseconds[2];
	} timestamp;
	u32 info32;
	u16 info16;
	u16 status;
};

struct fence {
	u32 hw_chid;
	u32 syncpt_val;
};

/* contexts associated with a channel */
struct channel_ctx_gk20a {
	struct gr_ctx_desc	gr_ctx;
	struct pm_ctx_desc	pm_ctx;
	struct patch_desc	patch_ctx;
	struct zcull_ctx_desc	zcull_ctx;
	u32	global_ctx_buffer_va[NR_GLOBAL_CTX_BUF_VA];
	bool	global_ctx_buffer_mapped;
};

/* this is the priv element of struct nvhost_channel */
struct channel_gk20a {
	struct gk20a *g;
	bool in_use;
	int hw_chid;
	bool bound;
	bool first_init;
	bool vpr;

	struct mem_mgr *memmgr;
	struct nvhost_channel *ch;
	struct nvhost_hwctx *hwctx;

	struct vm_gk20a *vm;

	struct gpfifo_desc gpfifo;

	struct channel_ctx_gk20a ch_ctx;

	struct inst_desc inst_block;
	struct mem_desc_sub ramfc;

	void *userd_cpu_va;
	phys_addr_t userd_cpu_pa;
	u64 userd_gpu_va;

	s32 num_objects;

	struct priv_cmd_queue priv_cmd_q;

	wait_queue_head_t notifier_wq;
	wait_queue_head_t semaphore_wq;

	void (*remove_support)(struct channel_gk20a *);
};

static inline bool gk20a_channel_as_bound(struct channel_gk20a *ch)
{
	return !!ch->hwctx->as_share;
}
int channel_gk20a_commit_va(struct channel_gk20a *c);

struct nvhost_unmap_buffer_args;
struct nvhost_zcull_get_size_args;
struct nvhost_zbc_query_table_args;
struct nvhost_fence;
struct nvhost_alloc_gpfifo_args;
struct nvhost_map_buffer_args;
struct nvhost_wait_args;
struct nvhost_zcull_bind_args;
struct nvhost_zcull_get_info_args;
struct nvhost_gpfifo;
struct nvhost_zbc_set_table_args;

int gk20a_init_channel_support(struct gk20a *, u32 chid);
int gk20a_channel_init(struct nvhost_channel *ch, struct nvhost_master *host,
		       int index);
int gk20a_channel_submit(struct nvhost_job *job);
int gk20a_channel_alloc_obj(struct nvhost_channel *channel,
			u32 class_num, u32 *obj_id, u32 vaspace_share);
int gk20a_channel_free_obj(struct nvhost_channel *channel,
			u32 obj_id);
struct nvhost_hwctx *gk20a_open_channel(struct nvhost_channel *ch,
			struct nvhost_hwctx *ctx);
int gk20a_alloc_channel_gpfifo(struct channel_gk20a *c,
			struct nvhost_alloc_gpfifo_args *args);
int gk20a_submit_channel_gpfifo(struct channel_gk20a *c,
			struct nvhost_gpfifo *gpfifo, u32 num_entries,
			struct nvhost_fence *fence, u32 flags);
void gk20a_free_channel(struct nvhost_hwctx *ctx);
int gk20a_channel_wait(struct channel_gk20a *ch,
		       struct nvhost_wait_args *args);
int gk20a_channel_zcull_get_size(struct channel_gk20a *ch,
			    struct nvhost_zcull_get_size_args *args);
int gk20a_channel_zcull_bind(struct channel_gk20a *ch,
			    struct nvhost_zcull_bind_args *args);
int gk20a_channel_zcull_get_info(struct channel_gk20a *ch,
			    struct nvhost_zcull_get_info_args *args);
int gk20a_channel_zbc_set_table(struct channel_gk20a *ch,
			    struct nvhost_zbc_set_table_args *args);
int gk20a_channel_zbc_query_table(struct channel_gk20a *ch,
			    struct nvhost_zbc_query_table_args *args);

static inline
struct mem_mgr *gk20a_channel_mem_mgr(struct channel_gk20a *ch)
{
	return ch->hwctx->memmgr;
}

static inline
struct nvhost_master *host_from_gk20a_channel(struct channel_gk20a *ch)
{
	return nvhost_get_host(ch->ch->dev);
}

#endif /*__CHANNEL_GK20A_H__*/
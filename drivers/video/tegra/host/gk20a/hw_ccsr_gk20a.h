/*
 * drivers/video/tegra/host/gk20a/hw_ccsr_gk20a.h
 *
 * Copyright (c) 2012, NVIDIA Corporation.
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
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

 /*
  * Function naming determines intended use:
  *
  *     <x>_r(void) : Returns the offset for register <x>.
  *
  *     <x>_w(void) : Returns the word offset for word (4 byte) element <x>.
  *
  *     <x>_<y>_s(void) : Returns size of field <y> of register <x> in bits.
  *
  *     <x>_<y>_f(u32 v) : Returns a value based on 'v' which has been shifted
  *         and masked to place it at field <y> of register <x>.  This value
  *         can be |'d with others to produce a full register value for
  *         register <x>.
  *
  *     <x>_<y>_m(void) : Returns a mask for field <y> of register <x>.  This
  *         value can be ~'d and then &'d to clear the value of field <y> for
  *         register <x>.
  *
  *     <x>_<y>_<z>_f(void) : Returns the constant value <z> after being shifted
  *         to place it at field <y> of register <x>.  This value can be |'d
  *         with others to produce a full register value for <x>.
  *
  *     <x>_<y>_v(u32 r) : Returns the value of field <y> from a full register
  *         <x> value 'r' after being shifted to place its LSB at bit 0.
  *         This value is suitable for direct comparison with other unshifted
  *         values appropriate for use in field <y> of register <x>.
  *
  *     <x>_<y>_<z>_v(void) : Returns the constant value for <z> defined for
  *         field <y> of register <x>.  This value is suitable for direct
  *         comparison with unshifted values appropriate for use in field <y>
  *         of register <x>.
  */

#ifndef __hw_ccsr_gk20a_h__
#define __hw_ccsr_gk20a_h__
/*This file is autogenerated.  Do not edit. */

static inline u32 ccsr_channel_inst_r(u32 i)
{
	return 0x00800000+(i)*8;
}
static inline u32 ccsr_channel_inst__size_1_v(void)
{
	return 128;
}
static inline u32 ccsr_channel_inst_ptr_s(void)
{
	return 28;
}
static inline u32 ccsr_channel_inst_ptr_f(u32 v)
{
	return (v & 0xfffffff) << 0;
}
static inline u32 ccsr_channel_inst_ptr_m(void)
{
	return 0xfffffff << 0;
}
static inline u32 ccsr_channel_inst_ptr_v(u32 r)
{
	return (r >> 0) & 0xfffffff;
}
static inline u32 ccsr_channel_inst_target_s(void)
{
	return 2;
}
static inline u32 ccsr_channel_inst_target_f(u32 v)
{
	return (v & 0x3) << 28;
}
static inline u32 ccsr_channel_inst_target_m(void)
{
	return 0x3 << 28;
}
static inline u32 ccsr_channel_inst_target_v(u32 r)
{
	return (r >> 28) & 0x3;
}
static inline u32 ccsr_channel_inst_target_vid_mem_v(void)
{
	return 0x00000000;
}
static inline u32 ccsr_channel_inst_target_vid_mem_f(void)
{
	return 0x0;
}
static inline u32 ccsr_channel_inst_target_sys_mem_coherent_v(void)
{
	return 0x00000002;
}
static inline u32 ccsr_channel_inst_target_sys_mem_coherent_f(void)
{
	return 0x20000000;
}
static inline u32 ccsr_channel_inst_target_sys_mem_noncoherent_v(void)
{
	return 0x00000003;
}
static inline u32 ccsr_channel_inst_target_sys_mem_noncoherent_f(void)
{
	return 0x30000000;
}
static inline u32 ccsr_channel_inst_bind_s(void)
{
	return 1;
}
static inline u32 ccsr_channel_inst_bind_f(u32 v)
{
	return (v & 0x1) << 31;
}
static inline u32 ccsr_channel_inst_bind_m(void)
{
	return 0x1 << 31;
}
static inline u32 ccsr_channel_inst_bind_v(u32 r)
{
	return (r >> 31) & 0x1;
}
static inline u32 ccsr_channel_inst_bind_false_v(void)
{
	return 0x00000000;
}
static inline u32 ccsr_channel_inst_bind_false_f(void)
{
	return 0x0;
}
static inline u32 ccsr_channel_inst_bind_true_v(void)
{
	return 0x00000001;
}
static inline u32 ccsr_channel_inst_bind_true_f(void)
{
	return 0x80000000;
}
static inline u32 ccsr_channel_inst_ptr_align_shift_v(void)
{
	return 12;
}
static inline u32 ccsr_channel_r(u32 i)
{
	return 0x00800004+(i)*8;
}
static inline u32 ccsr_channel__size_1_v(void)
{
	return 128;
}
static inline u32 ccsr_channel_enable_s(void)
{
	return 1;
}
static inline u32 ccsr_channel_enable_f(u32 v)
{
	return (v & 0x1) << 0;
}
static inline u32 ccsr_channel_enable_m(void)
{
	return 0x1 << 0;
}
static inline u32 ccsr_channel_enable_v(u32 r)
{
	return (r >> 0) & 0x1;
}
static inline u32 ccsr_channel_enable_not_in_use_v(void)
{
	return 0x00000000;
}
static inline u32 ccsr_channel_enable_not_in_use_f(void)
{
	return 0x0;
}
static inline u32 ccsr_channel_enable_in_use_v(void)
{
	return 0x00000001;
}
static inline u32 ccsr_channel_enable_in_use_f(void)
{
	return 0x1;
}
static inline u32 ccsr_channel_next_s(void)
{
	return 1;
}
static inline u32 ccsr_channel_next_f(u32 v)
{
	return (v & 0x1) << 1;
}
static inline u32 ccsr_channel_next_m(void)
{
	return 0x1 << 1;
}
static inline u32 ccsr_channel_next_v(u32 r)
{
	return (r >> 1) & 0x1;
}
static inline u32 ccsr_channel_next_false_v(void)
{
	return 0x00000000;
}
static inline u32 ccsr_channel_next_false_f(void)
{
	return 0x0;
}
static inline u32 ccsr_channel_next_true_v(void)
{
	return 0x00000001;
}
static inline u32 ccsr_channel_next_true_f(void)
{
	return 0x2;
}
static inline u32 ccsr_channel_force_pending_s(void)
{
	return 1;
}
static inline u32 ccsr_channel_force_pending_f(u32 v)
{
	return (v & 0x1) << 9;
}
static inline u32 ccsr_channel_force_pending_m(void)
{
	return 0x1 << 9;
}
static inline u32 ccsr_channel_force_pending_v(u32 r)
{
	return (r >> 9) & 0x1;
}
static inline u32 ccsr_channel_force_pending_true_v(void)
{
	return 0x00000001;
}
static inline u32 ccsr_channel_force_pending_true_f(void)
{
	return 0x200;
}
static inline u32 ccsr_channel_enable_set_s(void)
{
	return 1;
}
static inline u32 ccsr_channel_enable_set_f(u32 v)
{
	return (v & 0x1) << 10;
}
static inline u32 ccsr_channel_enable_set_m(void)
{
	return 0x1 << 10;
}
static inline u32 ccsr_channel_enable_set_v(u32 r)
{
	return (r >> 10) & 0x1;
}
static inline u32 ccsr_channel_enable_set_true_v(void)
{
	return 0x00000001;
}
static inline u32 ccsr_channel_enable_set_true_f(void)
{
	return 0x400;
}
static inline u32 ccsr_channel_enable_set_false_v(void)
{
	return 0x00000000;
}
static inline u32 ccsr_channel_enable_set_false_f(void)
{
	return 0x0;
}
static inline u32 ccsr_channel_enable_clr_s(void)
{
	return 1;
}
static inline u32 ccsr_channel_enable_clr_f(u32 v)
{
	return (v & 0x1) << 11;
}
static inline u32 ccsr_channel_enable_clr_m(void)
{
	return 0x1 << 11;
}
static inline u32 ccsr_channel_enable_clr_v(u32 r)
{
	return (r >> 11) & 0x1;
}
static inline u32 ccsr_channel_enable_clr_true_v(void)
{
	return 0x00000001;
}
static inline u32 ccsr_channel_enable_clr_true_f(void)
{
	return 0x800;
}
static inline u32 ccsr_channel_enable_clr_false_v(void)
{
	return 0x00000000;
}
static inline u32 ccsr_channel_enable_clr_false_f(void)
{
	return 0x0;
}
static inline u32 ccsr_channel_runlist_s(void)
{
	return 4;
}
static inline u32 ccsr_channel_runlist_f(u32 v)
{
	return (v & 0xf) << 16;
}
static inline u32 ccsr_channel_runlist_m(void)
{
	return 0xf << 16;
}
static inline u32 ccsr_channel_runlist_v(u32 r)
{
	return (r >> 16) & 0xf;
}
static inline u32 ccsr_channel_runlist_0_v(void)
{
	return 0x00000000;
}
static inline u32 ccsr_channel_runlist_0_f(void)
{
	return 0x0;
}
static inline u32 ccsr_channel_status_s(void)
{
	return 4;
}
static inline u32 ccsr_channel_status_f(u32 v)
{
	return (v & 0xf) << 24;
}
static inline u32 ccsr_channel_status_m(void)
{
	return 0xf << 24;
}
static inline u32 ccsr_channel_status_v(u32 r)
{
	return (r >> 24) & 0xf;
}
static inline u32 ccsr_channel_status_idle_v(void)
{
	return 0x00000000;
}
static inline u32 ccsr_channel_status_idle_f(void)
{
	return 0x0;
}
static inline u32 ccsr_channel_status_pending_v(void)
{
	return 0x00000001;
}
static inline u32 ccsr_channel_status_pending_f(void)
{
	return 0x1000000;
}
static inline u32 ccsr_channel_status_pending_ctx_reload_v(void)
{
	return 0x00000002;
}
static inline u32 ccsr_channel_status_pending_ctx_reload_f(void)
{
	return 0x2000000;
}
static inline u32 ccsr_channel_status_pending_acquire_v(void)
{
	return 0x00000003;
}
static inline u32 ccsr_channel_status_pending_acquire_f(void)
{
	return 0x3000000;
}
static inline u32 ccsr_channel_status_pending_acq_ctx_reload_v(void)
{
	return 0x00000004;
}
static inline u32 ccsr_channel_status_pending_acq_ctx_reload_f(void)
{
	return 0x4000000;
}
static inline u32 ccsr_channel_status_on_pbdma_v(void)
{
	return 0x00000005;
}
static inline u32 ccsr_channel_status_on_pbdma_f(void)
{
	return 0x5000000;
}
static inline u32 ccsr_channel_status_on_pbdma_and_eng_v(void)
{
	return 0x00000006;
}
static inline u32 ccsr_channel_status_on_pbdma_and_eng_f(void)
{
	return 0x6000000;
}
static inline u32 ccsr_channel_status_on_eng_v(void)
{
	return 0x00000007;
}
static inline u32 ccsr_channel_status_on_eng_f(void)
{
	return 0x7000000;
}
static inline u32 ccsr_channel_status_on_eng_pending_acquire_v(void)
{
	return 0x00000008;
}
static inline u32 ccsr_channel_status_on_eng_pending_acquire_f(void)
{
	return 0x8000000;
}
static inline u32 ccsr_channel_status_on_eng_pending_v(void)
{
	return 0x00000009;
}
static inline u32 ccsr_channel_status_on_eng_pending_f(void)
{
	return 0x9000000;
}
static inline u32 ccsr_channel_status_on_pbdma_ctx_reload_v(void)
{
	return 0x0000000A;
}
static inline u32 ccsr_channel_status_on_pbdma_ctx_reload_f(void)
{
	return 0xa000000;
}
static inline u32 ccsr_channel_status_on_pbdma_and_eng_ctx_reload_v(void)
{
	return 0x0000000B;
}
static inline u32 ccsr_channel_status_on_pbdma_and_eng_ctx_reload_f(void)
{
	return 0xb000000;
}
static inline u32 ccsr_channel_status_on_eng_ctx_reload_v(void)
{
	return 0x0000000C;
}
static inline u32 ccsr_channel_status_on_eng_ctx_reload_f(void)
{
	return 0xc000000;
}
static inline u32 ccsr_channel_status_on_eng_pending_ctx_reload_v(void)
{
	return 0x0000000D;
}
static inline u32 ccsr_channel_status_on_eng_pending_ctx_reload_f(void)
{
	return 0xd000000;
}
static inline u32 ccsr_channel_status_on_eng_pending_acq_ctx_reload_v(void)
{
	return 0x0000000E;
}
static inline u32 ccsr_channel_status_on_eng_pending_acq_ctx_reload_f(void)
{
	return 0xe000000;
}
static inline u32 ccsr_channel_busy_s(void)
{
	return 1;
}
static inline u32 ccsr_channel_busy_f(u32 v)
{
	return (v & 0x1) << 28;
}
static inline u32 ccsr_channel_busy_m(void)
{
	return 0x1 << 28;
}
static inline u32 ccsr_channel_busy_v(u32 r)
{
	return (r >> 28) & 0x1;
}
static inline u32 ccsr_channel_busy_false_v(void)
{
	return 0x00000000;
}
static inline u32 ccsr_channel_busy_false_f(void)
{
	return 0x0;
}
static inline u32 ccsr_channel_busy_true_v(void)
{
	return 0x00000001;
}
static inline u32 ccsr_channel_busy_true_f(void)
{
	return 0x10000000;
}

#endif /* __hw_ccsr_gk20a_h__ */
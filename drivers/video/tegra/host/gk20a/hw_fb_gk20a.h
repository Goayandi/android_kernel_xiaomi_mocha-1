/*
 * drivers/video/tegra/host/gk20a/hw_fb_gk20a.h
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

#ifndef __hw_fb_gk20a_h__
#define __hw_fb_gk20a_h__
/*This file is autogenerated.  Do not edit. */

static inline u32 fb_mmu_ctrl_r(void)
{
	return 0x00100C80;
}
static inline u32 fb_mmu_ctrl_vm_pg_size_s(void)
{
	return 1;
}
static inline u32 fb_mmu_ctrl_vm_pg_size_f(u32 v)
{
	return (v & 0x1) << 0;
}
static inline u32 fb_mmu_ctrl_vm_pg_size_m(void)
{
	return 0x1 << 0;
}
static inline u32 fb_mmu_ctrl_vm_pg_size_v(u32 r)
{
	return (r >> 0) & 0x1;
}
static inline u32 fb_mmu_ctrl_vm_pg_size_128kb_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_ctrl_vm_pg_size_128kb_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_ctrl_vm_pg_size_64kb_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_ctrl_vm_pg_size_64kb_f(void)
{
	return 0x1;
}
static inline u32 fb_mmu_ctrl_vol_fault_s(void)
{
	return 1;
}
static inline u32 fb_mmu_ctrl_vol_fault_f(u32 v)
{
	return (v & 0x1) << 1;
}
static inline u32 fb_mmu_ctrl_vol_fault_m(void)
{
	return 0x1 << 1;
}
static inline u32 fb_mmu_ctrl_vol_fault_v(u32 r)
{
	return (r >> 1) & 0x1;
}
static inline u32 fb_mmu_ctrl_vol_fault_enabled_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_ctrl_vol_fault_enabled_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_ctrl_vol_fault_disabled_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_ctrl_vol_fault_disabled_f(void)
{
	return 0x2;
}
static inline u32 fb_mmu_ctrl_comp_fault_s(void)
{
	return 1;
}
static inline u32 fb_mmu_ctrl_comp_fault_f(u32 v)
{
	return (v & 0x1) << 2;
}
static inline u32 fb_mmu_ctrl_comp_fault_m(void)
{
	return 0x1 << 2;
}
static inline u32 fb_mmu_ctrl_comp_fault_v(u32 r)
{
	return (r >> 2) & 0x1;
}
static inline u32 fb_mmu_ctrl_comp_fault_enabled_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_ctrl_comp_fault_enabled_f(void)
{
	return 0x4;
}
static inline u32 fb_mmu_ctrl_comp_fault_disabled_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_ctrl_comp_fault_disabled_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_ctrl_miss_gran_s(void)
{
	return 2;
}
static inline u32 fb_mmu_ctrl_miss_gran_f(u32 v)
{
	return (v & 0x3) << 3;
}
static inline u32 fb_mmu_ctrl_miss_gran_m(void)
{
	return 0x3 << 3;
}
static inline u32 fb_mmu_ctrl_miss_gran_v(u32 r)
{
	return (r >> 3) & 0x3;
}
static inline u32 fb_mmu_ctrl_miss_gran_full_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_ctrl_miss_gran_full_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_ctrl_miss_gran_half_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_ctrl_miss_gran_half_f(void)
{
	return 0x8;
}
static inline u32 fb_mmu_ctrl_miss_gran_quarter_v(void)
{
	return 0x00000002;
}
static inline u32 fb_mmu_ctrl_miss_gran_quarter_f(void)
{
	return 0x10;
}
static inline u32 fb_mmu_ctrl_cache_mode_s(void)
{
	return 2;
}
static inline u32 fb_mmu_ctrl_cache_mode_f(u32 v)
{
	return (v & 0x3) << 5;
}
static inline u32 fb_mmu_ctrl_cache_mode_m(void)
{
	return 0x3 << 5;
}
static inline u32 fb_mmu_ctrl_cache_mode_v(u32 r)
{
	return (r >> 5) & 0x3;
}
static inline u32 fb_mmu_ctrl_cache_mode_on_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_ctrl_cache_mode_on_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_ctrl_cache_mode_evict_smpg_partials_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_ctrl_cache_mode_evict_smpg_partials_f(void)
{
	return 0x20;
}
static inline u32 fb_mmu_ctrl_cache_mode_evict_all_partials_v(void)
{
	return 0x00000002;
}
static inline u32 fb_mmu_ctrl_cache_mode_evict_all_partials_f(void)
{
	return 0x40;
}
static inline u32 fb_mmu_ctrl_cache_mode_off_v(void)
{
	return 0x00000003;
}
static inline u32 fb_mmu_ctrl_cache_mode_off_f(void)
{
	return 0x60;
}
static inline u32 fb_mmu_ctrl_pde_cache_mode_s(void)
{
	return 2;
}
static inline u32 fb_mmu_ctrl_pde_cache_mode_f(u32 v)
{
	return (v & 0x3) << 7;
}
static inline u32 fb_mmu_ctrl_pde_cache_mode_m(void)
{
	return 0x3 << 7;
}
static inline u32 fb_mmu_ctrl_pde_cache_mode_v(u32 r)
{
	return (r >> 7) & 0x3;
}
static inline u32 fb_mmu_ctrl_pde_cache_mode_on_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_ctrl_pde_cache_mode_on_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_ctrl_pde_cache_mode_evict_smpg_partials_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_ctrl_pde_cache_mode_evict_smpg_partials_f(void)
{
	return 0x80;
}
static inline u32 fb_mmu_ctrl_pde_cache_mode_evict_all_partials_v(void)
{
	return 0x00000002;
}
static inline u32 fb_mmu_ctrl_pde_cache_mode_evict_all_partials_f(void)
{
	return 0x100;
}
static inline u32 fb_mmu_ctrl_pde_cache_mode_off_v(void)
{
	return 0x00000003;
}
static inline u32 fb_mmu_ctrl_pde_cache_mode_off_f(void)
{
	return 0x180;
}
static inline u32 fb_mmu_ctrl_l2_send_mode_s(void)
{
	return 1;
}
static inline u32 fb_mmu_ctrl_l2_send_mode_f(u32 v)
{
	return (v & 0x1) << 9;
}
static inline u32 fb_mmu_ctrl_l2_send_mode_m(void)
{
	return 0x1 << 9;
}
static inline u32 fb_mmu_ctrl_l2_send_mode_v(u32 r)
{
	return (r >> 9) & 0x1;
}
static inline u32 fb_mmu_ctrl_l2_send_mode_one_pte_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_ctrl_l2_send_mode_one_pte_f(void)
{
	return 0x200;
}
static inline u32 fb_mmu_ctrl_l2_send_mode_whole_cl_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_ctrl_l2_send_mode_whole_cl_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_ctrl_work_creation_disable_s(void)
{
	return 1;
}
static inline u32 fb_mmu_ctrl_work_creation_disable_f(u32 v)
{
	return (v & 0x1) << 10;
}
static inline u32 fb_mmu_ctrl_work_creation_disable_m(void)
{
	return 0x1 << 10;
}
static inline u32 fb_mmu_ctrl_work_creation_disable_v(u32 r)
{
	return (r >> 10) & 0x1;
}
static inline u32 fb_mmu_ctrl_work_creation_disable_false_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_ctrl_work_creation_disable_false_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_ctrl_work_creation_disable_true_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_ctrl_work_creation_disable_true_f(void)
{
	return 0x400;
}
static inline u32 fb_mmu_ctrl_pri_fifo_error_clear_s(void)
{
	return 1;
}
static inline u32 fb_mmu_ctrl_pri_fifo_error_clear_f(u32 v)
{
	return (v & 0x1) << 13;
}
static inline u32 fb_mmu_ctrl_pri_fifo_error_clear_m(void)
{
	return 0x1 << 13;
}
static inline u32 fb_mmu_ctrl_pri_fifo_error_clear_v(u32 r)
{
	return (r >> 13) & 0x1;
}
static inline u32 fb_mmu_ctrl_pri_fifo_error_clear_true_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_ctrl_pri_fifo_error_clear_true_f(void)
{
	return 0x2000;
}
static inline u32 fb_mmu_ctrl_pri_fifo_error_s(void)
{
	return 1;
}
static inline u32 fb_mmu_ctrl_pri_fifo_error_f(u32 v)
{
	return (v & 0x1) << 14;
}
static inline u32 fb_mmu_ctrl_pri_fifo_error_m(void)
{
	return 0x1 << 14;
}
static inline u32 fb_mmu_ctrl_pri_fifo_error_v(u32 r)
{
	return (r >> 14) & 0x1;
}
static inline u32 fb_mmu_ctrl_pri_fifo_error_false_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_ctrl_pri_fifo_error_false_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_ctrl_pri_fifo_error_true_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_ctrl_pri_fifo_error_true_f(void)
{
	return 0x4000;
}
static inline u32 fb_mmu_ctrl_pri_fifo_empty_s(void)
{
	return 1;
}
static inline u32 fb_mmu_ctrl_pri_fifo_empty_f(u32 v)
{
	return (v & 0x1) << 15;
}
static inline u32 fb_mmu_ctrl_pri_fifo_empty_m(void)
{
	return 0x1 << 15;
}
static inline u32 fb_mmu_ctrl_pri_fifo_empty_v(u32 r)
{
	return (r >> 15) & 0x1;
}
static inline u32 fb_mmu_ctrl_pri_fifo_empty_false_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_ctrl_pri_fifo_empty_false_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_ctrl_pri_fifo_empty_true_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_ctrl_pri_fifo_empty_true_f(void)
{
	return 0x8000;
}
static inline u32 fb_mmu_ctrl_pri_fifo_space_s(void)
{
	return 8;
}
static inline u32 fb_mmu_ctrl_pri_fifo_space_f(u32 v)
{
	return (v & 0xff) << 16;
}
static inline u32 fb_mmu_ctrl_pri_fifo_space_m(void)
{
	return 0xff << 16;
}
static inline u32 fb_mmu_ctrl_pri_fifo_space_v(u32 r)
{
	return (r >> 16) & 0xff;
}
static inline u32 fb_mmu_ctrl_pri_fifo_space_init_v(void)
{
	return 32;
}
static inline u32 fb_mmu_ctrl_pri_fifo_space_init_f(void)
{
	return 0x200000;
}
static inline u32 fb_mmu_ctrl_mmu_aperture_s(void)
{
	return 2;
}
static inline u32 fb_mmu_ctrl_mmu_aperture_f(u32 v)
{
	return (v & 0x3) << 28;
}
static inline u32 fb_mmu_ctrl_mmu_aperture_m(void)
{
	return 0x3 << 28;
}
static inline u32 fb_mmu_ctrl_mmu_aperture_v(u32 r)
{
	return (r >> 28) & 0x3;
}
static inline u32 fb_mmu_ctrl_mmu_aperture_vid_mem_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_ctrl_mmu_aperture_vid_mem_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_ctrl_mmu_aperture_peer_mem_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_ctrl_mmu_aperture_peer_mem_f(void)
{
	return 0x10000000;
}
static inline u32 fb_mmu_ctrl_mmu_aperture_sys_mem_c_v(void)
{
	return 0x00000002;
}
static inline u32 fb_mmu_ctrl_mmu_aperture_sys_mem_c_f(void)
{
	return 0x20000000;
}
static inline u32 fb_mmu_ctrl_mmu_aperture_sys_mem_nc_v(void)
{
	return 0x00000003;
}
static inline u32 fb_mmu_ctrl_mmu_aperture_sys_mem_nc_f(void)
{
	return 0x30000000;
}
static inline u32 fb_mmu_ctrl_mmu_vol_s(void)
{
	return 1;
}
static inline u32 fb_mmu_ctrl_mmu_vol_f(u32 v)
{
	return (v & 0x1) << 30;
}
static inline u32 fb_mmu_ctrl_mmu_vol_m(void)
{
	return 0x1 << 30;
}
static inline u32 fb_mmu_ctrl_mmu_vol_v(u32 r)
{
	return (r >> 30) & 0x1;
}
static inline u32 fb_mmu_ctrl_mmu_vol_false_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_ctrl_mmu_vol_false_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_ctrl_mmu_vol_true_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_ctrl_mmu_vol_true_f(void)
{
	return 0x40000000;
}
static inline u32 fb_mmu_ctrl_mmu_disable_s(void)
{
	return 1;
}
static inline u32 fb_mmu_ctrl_mmu_disable_f(u32 v)
{
	return (v & 0x1) << 31;
}
static inline u32 fb_mmu_ctrl_mmu_disable_m(void)
{
	return 0x1 << 31;
}
static inline u32 fb_mmu_ctrl_mmu_disable_v(u32 r)
{
	return (r >> 31) & 0x1;
}
static inline u32 fb_mmu_ctrl_mmu_disable_false_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_ctrl_mmu_disable_false_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_ctrl_mmu_disable_true_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_ctrl_mmu_disable_true_f(void)
{
	return 0x80000000;
}
static inline u32 fb_mmu_invalidate_pdb_r(void)
{
	return 0x00100CB8;
}
static inline u32 fb_mmu_invalidate_pdb_aperture_s(void)
{
	return 1;
}
static inline u32 fb_mmu_invalidate_pdb_aperture_f(u32 v)
{
	return (v & 0x1) << 1;
}
static inline u32 fb_mmu_invalidate_pdb_aperture_m(void)
{
	return 0x1 << 1;
}
static inline u32 fb_mmu_invalidate_pdb_aperture_v(u32 r)
{
	return (r >> 1) & 0x1;
}
static inline u32 fb_mmu_invalidate_pdb_aperture_vid_mem_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_invalidate_pdb_aperture_vid_mem_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_invalidate_pdb_aperture_sys_mem_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_invalidate_pdb_aperture_sys_mem_f(void)
{
	return 0x2;
}
static inline u32 fb_mmu_invalidate_pdb_addr_s(void)
{
	return 28;
}
static inline u32 fb_mmu_invalidate_pdb_addr_f(u32 v)
{
	return (v & 0xfffffff) << 4;
}
static inline u32 fb_mmu_invalidate_pdb_addr_m(void)
{
	return 0xfffffff << 4;
}
static inline u32 fb_mmu_invalidate_pdb_addr_v(u32 r)
{
	return (r >> 4) & 0xfffffff;
}
static inline u32 fb_mmu_invalidate_pdb_addr_alignment_v(void)
{
	return 0x0000000c;
}
static inline u32 fb_mmu_invalidate_pdb_addr_alignment_f(void)
{
	return 0xc0;
}
static inline u32 fb_mmu_invalidate_r(void)
{
	return 0x00100CBC;
}
static inline u32 fb_mmu_invalidate_all_va_s(void)
{
	return 1;
}
static inline u32 fb_mmu_invalidate_all_va_f(u32 v)
{
	return (v & 0x1) << 0;
}
static inline u32 fb_mmu_invalidate_all_va_m(void)
{
	return 0x1 << 0;
}
static inline u32 fb_mmu_invalidate_all_va_v(u32 r)
{
	return (r >> 0) & 0x1;
}
static inline u32 fb_mmu_invalidate_all_va_false_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_invalidate_all_va_false_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_invalidate_all_va_true_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_invalidate_all_va_true_f(void)
{
	return 0x1;
}
static inline u32 fb_mmu_invalidate_all_pdb_s(void)
{
	return 1;
}
static inline u32 fb_mmu_invalidate_all_pdb_f(u32 v)
{
	return (v & 0x1) << 1;
}
static inline u32 fb_mmu_invalidate_all_pdb_m(void)
{
	return 0x1 << 1;
}
static inline u32 fb_mmu_invalidate_all_pdb_v(u32 r)
{
	return (r >> 1) & 0x1;
}
static inline u32 fb_mmu_invalidate_all_pdb_false_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_invalidate_all_pdb_false_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_invalidate_all_pdb_true_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_invalidate_all_pdb_true_f(void)
{
	return 0x2;
}
static inline u32 fb_mmu_invalidate_hubtlb_only_s(void)
{
	return 1;
}
static inline u32 fb_mmu_invalidate_hubtlb_only_f(u32 v)
{
	return (v & 0x1) << 2;
}
static inline u32 fb_mmu_invalidate_hubtlb_only_m(void)
{
	return 0x1 << 2;
}
static inline u32 fb_mmu_invalidate_hubtlb_only_v(u32 r)
{
	return (r >> 2) & 0x1;
}
static inline u32 fb_mmu_invalidate_hubtlb_only_false_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_invalidate_hubtlb_only_false_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_invalidate_hubtlb_only_true_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_invalidate_hubtlb_only_true_f(void)
{
	return 0x4;
}
static inline u32 fb_mmu_invalidate_trigger_s(void)
{
	return 1;
}
static inline u32 fb_mmu_invalidate_trigger_f(u32 v)
{
	return (v & 0x1) << 31;
}
static inline u32 fb_mmu_invalidate_trigger_m(void)
{
	return 0x1 << 31;
}
static inline u32 fb_mmu_invalidate_trigger_v(u32 r)
{
	return (r >> 31) & 0x1;
}
static inline u32 fb_mmu_invalidate_trigger_false_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_invalidate_trigger_false_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_invalidate_trigger_true_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_invalidate_trigger_true_f(void)
{
	return 0x80000000;
}
static inline u32 fb_mmu_invalidate_max_cacheline_size_v(void)
{
	return 0x00000010;
}
static inline u32 fb_mmu_debug_wr_r(void)
{
	return 0x00100CC8;
}
static inline u32 fb_mmu_debug_wr_aperture_s(void)
{
	return 2;
}
static inline u32 fb_mmu_debug_wr_aperture_f(u32 v)
{
	return (v & 0x3) << 0;
}
static inline u32 fb_mmu_debug_wr_aperture_m(void)
{
	return 0x3 << 0;
}
static inline u32 fb_mmu_debug_wr_aperture_v(u32 r)
{
	return (r >> 0) & 0x3;
}
static inline u32 fb_mmu_debug_wr_aperture_vid_mem_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_debug_wr_aperture_vid_mem_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_debug_wr_aperture_sys_mem_c_v(void)
{
	return 0x00000002;
}
static inline u32 fb_mmu_debug_wr_aperture_sys_mem_c_f(void)
{
	return 0x2;
}
static inline u32 fb_mmu_debug_wr_aperture_sys_mem_nc_v(void)
{
	return 0x00000003;
}
static inline u32 fb_mmu_debug_wr_aperture_sys_mem_nc_f(void)
{
	return 0x3;
}
static inline u32 fb_mmu_debug_wr_vol_s(void)
{
	return 1;
}
static inline u32 fb_mmu_debug_wr_vol_f(u32 v)
{
	return (v & 0x1) << 2;
}
static inline u32 fb_mmu_debug_wr_vol_m(void)
{
	return 0x1 << 2;
}
static inline u32 fb_mmu_debug_wr_vol_v(u32 r)
{
	return (r >> 2) & 0x1;
}
static inline u32 fb_mmu_debug_wr_vol_false_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_debug_wr_vol_false_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_debug_wr_vol_true_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_debug_wr_vol_true_f(void)
{
	return 0x4;
}
static inline u32 fb_mmu_debug_wr_addr_s(void)
{
	return 28;
}
static inline u32 fb_mmu_debug_wr_addr_f(u32 v)
{
	return (v & 0xfffffff) << 4;
}
static inline u32 fb_mmu_debug_wr_addr_m(void)
{
	return 0xfffffff << 4;
}
static inline u32 fb_mmu_debug_wr_addr_v(u32 r)
{
	return (r >> 4) & 0xfffffff;
}
static inline u32 fb_mmu_debug_wr_addr_alignment_v(void)
{
	return 0x0000000c;
}
static inline u32 fb_mmu_debug_wr_addr_alignment_f(void)
{
	return 0xc0;
}
static inline u32 fb_mmu_debug_rd_r(void)
{
	return 0x00100CCC;
}
static inline u32 fb_mmu_debug_rd_aperture_s(void)
{
	return 2;
}
static inline u32 fb_mmu_debug_rd_aperture_f(u32 v)
{
	return (v & 0x3) << 0;
}
static inline u32 fb_mmu_debug_rd_aperture_m(void)
{
	return 0x3 << 0;
}
static inline u32 fb_mmu_debug_rd_aperture_v(u32 r)
{
	return (r >> 0) & 0x3;
}
static inline u32 fb_mmu_debug_rd_aperture_vid_mem_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_debug_rd_aperture_vid_mem_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_debug_rd_aperture_sys_mem_c_v(void)
{
	return 0x00000002;
}
static inline u32 fb_mmu_debug_rd_aperture_sys_mem_c_f(void)
{
	return 0x2;
}
static inline u32 fb_mmu_debug_rd_aperture_sys_mem_nc_v(void)
{
	return 0x00000003;
}
static inline u32 fb_mmu_debug_rd_aperture_sys_mem_nc_f(void)
{
	return 0x3;
}
static inline u32 fb_mmu_debug_rd_vol_s(void)
{
	return 1;
}
static inline u32 fb_mmu_debug_rd_vol_f(u32 v)
{
	return (v & 0x1) << 2;
}
static inline u32 fb_mmu_debug_rd_vol_m(void)
{
	return 0x1 << 2;
}
static inline u32 fb_mmu_debug_rd_vol_v(u32 r)
{
	return (r >> 2) & 0x1;
}
static inline u32 fb_mmu_debug_rd_vol_false_v(void)
{
	return 0x00000000;
}
static inline u32 fb_mmu_debug_rd_vol_false_f(void)
{
	return 0x0;
}
static inline u32 fb_mmu_debug_rd_vol_true_v(void)
{
	return 0x00000001;
}
static inline u32 fb_mmu_debug_rd_vol_true_f(void)
{
	return 0x4;
}
static inline u32 fb_mmu_debug_rd_addr_s(void)
{
	return 28;
}
static inline u32 fb_mmu_debug_rd_addr_f(u32 v)
{
	return (v & 0xfffffff) << 4;
}
static inline u32 fb_mmu_debug_rd_addr_m(void)
{
	return 0xfffffff << 4;
}
static inline u32 fb_mmu_debug_rd_addr_v(u32 r)
{
	return (r >> 4) & 0xfffffff;
}
static inline u32 fb_mmu_debug_rd_addr_alignment_v(void)
{
	return 0x0000000c;
}
static inline u32 fb_mmu_debug_rd_addr_alignment_f(void)
{
	return 0xc0;
}

#endif /* __hw_fb_gk20a_h__ */
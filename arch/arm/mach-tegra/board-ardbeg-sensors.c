/*
 * arch/arm/mach-tegra/board-ardbeg-sensors.c
 *
 * Copyright (c) 2013-2014, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */

#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/mpu.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/nct1008.h>
#include <linux/pid_thermal_gov.h>
#include <linux/tegra-fuse.h>
#include <mach/edp.h>
#include <mach/pinmux-t12.h>
#include <mach/pinmux.h>
#include <mach/io_dpd.h>
#include <media/camera.h>
#include <media/ar0261.h>
#include <media/imx179.h>
#include <media/dw9718.h>
#include <media/as364x.h>
#include <media/ov5693.h>
#include <media/ov7695.h>
#include <media/mt9m114.h>
#include <media/ad5823.h>
#include <media/max77387.h>

#include <linux/platform_device.h>
#include <media/soc_camera.h>
#include <media/soc_camera_platform.h>
#include <media/tegra_v4l2_camera.h>
#include <linux/generic_adc_thermal.h>

#include "cpu-tegra.h"
#include "devices.h"
#include "board.h"
#include "board-common.h"
#include "board-ardbeg.h"
#include "tegra-board-id.h"

#if defined(ARCH_TEGRA_12x_SOC)
static struct i2c_board_info ardbeg_i2c_board_info_cm32181[] = {
	{
		I2C_BOARD_INFO("cm32181", 0x48),
	},
};
#endif

/*
 * Soc Camera platform driver for testing
 */
#if IS_ENABLED(CONFIG_SOC_CAMERA_PLATFORM)
static int ardbeg_soc_camera_add(struct soc_camera_device *icd);
static void ardbeg_soc_camera_del(struct soc_camera_device *icd);

static int ardbeg_soc_camera_set_capture(struct soc_camera_platform_info *info,
		int enable)
{
	/* TODO: probably add clk opertaion here */
	return 0; /* camera sensor always enabled */
}

static struct soc_camera_platform_info ardbeg_soc_camera_info = {
	.format_name = "RGB4",
	.format_depth = 32,
	.format = {
		.code = V4L2_MBUS_FMT_RGBA8888_4X8_LE,
		.colorspace = V4L2_COLORSPACE_SRGB,
		.field = V4L2_FIELD_NONE,
		.width = 1280,
		.height = 720,
	},
	.set_capture = ardbeg_soc_camera_set_capture,
};

static struct tegra_camera_platform_data ardbeg_camera_platform_data = {
	.flip_v                 = 0,
	.flip_h                 = 0,
	.port                   = TEGRA_CAMERA_PORT_CSI_A,
	.lanes                  = 4,
	.continuous_clk         = 0,
};

static struct soc_camera_link ardbeg_soc_camera_link = {
	.bus_id         = 0, /* This must match the .id of tegra_vi01_device */
	.add_device     = ardbeg_soc_camera_add,
	.del_device     = ardbeg_soc_camera_del,
	.module_name    = "soc_camera_platform",
	.priv		= &ardbeg_camera_platform_data,
	.dev_priv	= &ardbeg_soc_camera_info,
};

static struct platform_device *ardbeg_pdev;

static void ardbeg_soc_camera_release(struct device *dev)
{
	soc_camera_platform_release(&ardbeg_pdev);
}

static int ardbeg_soc_camera_add(struct soc_camera_device *icd)
{
	return soc_camera_platform_add(icd, &ardbeg_pdev,
			&ardbeg_soc_camera_link,
			ardbeg_soc_camera_release, 0);
}

static void ardbeg_soc_camera_del(struct soc_camera_device *icd)
{
	soc_camera_platform_del(icd, ardbeg_pdev, &ardbeg_soc_camera_link);
}

static struct platform_device ardbeg_soc_camera_device = {
	.name   = "soc-camera-pdrv",
	.id     = 0,
	.dev    = {
		.platform_data = &ardbeg_soc_camera_link,
	},
};
#endif

static struct regulator *ov5693_1v2;
static struct regulator *ov5693_1v8;
static struct regulator *ov5693_afvdd;

static int ardbeg_get_extra_regulators(void)
{
	if (!ov5693_1v2) {
		ov5693_1v2 = regulator_get(NULL, "vdd_cam_1v2");
		if (WARN_ON(IS_ERR(ov5693_1v2))) {
			pr_err("%s: can't get regulator ov5693_1v2: %ld\n",
				__func__, PTR_ERR(ov5693_1v2));
			ov5693_1v2 = NULL;
			return -ENODEV;
		}
	}

	if (!ov5693_1v8) {
		ov5693_1v8 = regulator_get(NULL, "vdd_cam_1v8");
		if (WARN_ON(IS_ERR(ov5693_1v8))) {
			pr_err("%s: can't get regulator ov5693_1v8: %ld\n",
				__func__, PTR_ERR(ov5693_1v8));
			ov5693_1v8 = NULL;
			return -ENODEV;
		}
	}
	if (!ov5693_afvdd) {
		ov5693_afvdd = regulator_get(NULL, "imx179_reg1");
		if (WARN_ON(IS_ERR(ov5693_afvdd))) {
			pr_err("%s: can't get regulator ov5693_afvdd: %ld\n",
				__func__, PTR_ERR(ov5693_afvdd));
			ov5693_afvdd = NULL;
			return -ENODEV;
		}
	}

	return 0;
}

static struct tegra_io_dpd csia_io = {
	.name			= "CSIA",
	.io_dpd_reg_index	= 0,
	.io_dpd_bit		= 0,
};

static struct tegra_io_dpd csib_io = {
	.name			= "CSIB",
	.io_dpd_reg_index	= 0,
	.io_dpd_bit		= 1,
};

static struct tegra_io_dpd csie_io = {
	.name			= "CSIE",
	.io_dpd_reg_index	= 1,
	.io_dpd_bit		= 12,
};

static int ardbeg_ar0261_power_on(struct ar0261_power_rail *pw)
{
	int err;

	if (unlikely(WARN_ON(!pw || !pw->avdd || !pw->iovdd || !pw->dvdd)))
		return -EFAULT;

	/* disable CSIE IOs DPD mode to turn on front camera for ardbeg */
	tegra_io_dpd_disable(&csie_io);

	if (ardbeg_get_extra_regulators())
		goto ardbeg_ar0261_poweron_fail;

	gpio_set_value(CAM_RSTN, 0);
	gpio_set_value(CAM_AF_PWDN, 1);

	err = regulator_enable(pw->dvdd);
	if (unlikely(err))
		goto ar0261_dvdd_fail;

	err = regulator_enable(pw->avdd);
	if (unlikely(err))
		goto ar0261_avdd_fail;

	err = regulator_enable(pw->iovdd);
	if (unlikely(err))
		goto ar0261_iovdd_fail;

	usleep_range(1, 2);
	gpio_set_value(CAM2_PWDN, 1);

	gpio_set_value(CAM_RSTN, 1);

	return 0;
ar0261_iovdd_fail:
	regulator_disable(pw->dvdd);

ar0261_dvdd_fail:
	regulator_disable(pw->avdd);

ar0261_avdd_fail:
	pr_err("%s vcmvdd failed.\n", __func__);
	return -ENODEV;

ardbeg_ar0261_poweron_fail:
	/* put CSIE IOs into DPD mode to save additional power for ardbeg */
	tegra_io_dpd_enable(&csie_io);
	pr_err("%s failed.\n", __func__);
	return -ENODEV;
}

static int ardbeg_ar0261_power_off(struct ar0261_power_rail *pw)
{
	if (unlikely(WARN_ON(!pw || !pw->avdd || !pw->iovdd || !pw->dvdd))) {
		/* put CSIE IOs into DPD mode to
		 * save additional power for ardbeg
		 */
		tegra_io_dpd_enable(&csie_io);
		return -EFAULT;
	}

	gpio_set_value(CAM_RSTN, 0);

	usleep_range(1, 2);

	regulator_disable(pw->iovdd);
	regulator_disable(pw->dvdd);
	regulator_disable(pw->avdd);
	/* put CSIE IOs into DPD mode to save additional power for ardbeg */
	tegra_io_dpd_enable(&csie_io);
	return 0;
}

struct ar0261_platform_data ardbeg_ar0261_data = {
	.power_on = ardbeg_ar0261_power_on,
	.power_off = ardbeg_ar0261_power_off,
	.mclk_name = "mclk2",
};

static int ardbeg_imx179_get_extra_regulators(struct imx179_power_rail *pw)
{
	if (!pw->ext_reg1) {
		pw->ext_reg1 = regulator_get(NULL, "imx179_reg1");
		if (WARN_ON(IS_ERR(pw->ext_reg1))) {
			pr_err("%s: can't get regulator imx179_reg1: %ld\n",
				__func__, PTR_ERR(pw->ext_reg1));
			pw->ext_reg1 = NULL;
			return -ENODEV;
		}
	}

	if (!pw->ext_reg2) {
		pw->ext_reg2 = regulator_get(NULL, "vdd_cam_1v2");
		if (WARN_ON(IS_ERR(pw->ext_reg2))) {
			pr_err("%s: can't get regulator imx179_reg2: %ld\n",
				__func__, PTR_ERR(pw->ext_reg2));
			pw->ext_reg2 = NULL;
			return -ENODEV;
		}
	}
	if (!pw->ext_reg3) {
		pw->ext_reg3 = regulator_get(NULL, "vdd_cam_1v8");
		if (WARN_ON(IS_ERR(pw->ext_reg3))) {
			pr_err("%s: can't get regulator imx179_reg3: %ld\n",
				__func__, PTR_ERR(pw->ext_reg3));
			pw->ext_reg3 = NULL;
			return -ENODEV;
		}
	}

	return 0;
}

static int ardbeg_imx179_power_on(struct imx179_power_rail *pw)
{
	int err;
	
	pr_info("%s\n", __func__);
	if (unlikely(WARN_ON(!pw || !pw->iovdd || !pw->avdd)))
		return -EFAULT;

	/* disable CSIA/B IOs DPD mode to turn on camera for ardbeg */
	tegra_io_dpd_disable(&csia_io);
	tegra_io_dpd_disable(&csib_io);

	if (ardbeg_imx179_get_extra_regulators(pw))
		goto imx179_poweron_fail;

	err = regulator_enable(pw->ext_reg1);
	if (unlikely(err))
		goto imx179_ext_reg1_fail;

	err = regulator_enable(pw->ext_reg2);
	if (unlikely(err))
		goto imx179_ext_reg2_fail;


	err = regulator_enable(pw->ext_reg3);
	if (unlikely(err))
		goto imx179_ext_reg3_fail;

	err = regulator_enable(pw->avdd);
	if (err)
		goto imx179_avdd_fail;

	usleep_range(1, 2);
	
	gpio_set_value(CAM_AF_PWDN, 1);
	gpio_set_value(CAM_RSTN, 0);
	usleep_range(10, 20);

	gpio_set_value(CAM_RSTN, 1);

	usleep_range(300, 310);

	pr_info("%s finished\n", __func__);
	return 0;


imx179_avdd_fail:
	if (pw->ext_reg3)
		regulator_disable(pw->ext_reg3);

imx179_ext_reg3_fail:
	if (pw->ext_reg2)
		regulator_disable(pw->ext_reg2);
	gpio_set_value(CAM_AF_PWDN, 0);

imx179_ext_reg2_fail:
	if (pw->ext_reg1)
		regulator_disable(pw->ext_reg1);
	gpio_set_value(CAM_AF_PWDN, 0);

imx179_ext_reg1_fail:
imx179_poweron_fail:
	tegra_io_dpd_enable(&csia_io);
	tegra_io_dpd_enable(&csib_io);
	pr_err("%s failed.\n", __func__);
	return -ENODEV;
}

static int ardbeg_imx179_power_off(struct imx179_power_rail *pw)
{
	if (unlikely(WARN_ON(!pw || !pw->iovdd || !pw->avdd))) {
		tegra_io_dpd_enable(&csia_io);
		tegra_io_dpd_enable(&csib_io);
		return -EFAULT;
	}

	gpio_set_value(CAM_RSTN, 0);
	gpio_set_value(CAM_AF_PWDN, 0);
	usleep_range(1, 2);
	regulator_disable(pw->avdd);

	regulator_disable(pw->ext_reg1);
	regulator_disable(pw->ext_reg2);
	regulator_disable(pw->ext_reg3);

	/* put CSIA/B IOs into DPD mode to save additional power for ardbeg */
	tegra_io_dpd_enable(&csia_io);
	tegra_io_dpd_enable(&csib_io);
	pr_info("%s finished\n", __func__);
	return 0;
}

struct imx179_platform_data ardbeg_imx179_data = {
	.flash_cap = {
		.enable = 1,
		.edge_trig_en = 1,
		.start_edge = 0,
		.repeat = 1,
		.delay_frm = 0,
	},
	.power_on = ardbeg_imx179_power_on,
	.power_off = ardbeg_imx179_power_off,
};

static int ardbeg_ov7695_power_on(struct ov7695_power_rail *pw)
{
	int err;

	if (unlikely(WARN_ON(!pw || !pw->avdd || !pw->iovdd)))
		return -EFAULT;

	/* disable CSIE IOs DPD mode to turn on front camera for ardbeg */
	tegra_io_dpd_disable(&csie_io);

	gpio_set_value(CAM2_PWDN, 0);
	usleep_range(1000, 1020);

	err = regulator_enable(pw->avdd);
	if (unlikely(err))
		goto ov7695_avdd_fail;
	usleep_range(300, 320);

	err = regulator_enable(pw->iovdd);
	if (unlikely(err))
		goto ov7695_iovdd_fail;
	usleep_range(1000, 1020);

	gpio_set_value(CAM2_PWDN, 1);
	usleep_range(1000, 1020);

	return 0;

ov7695_iovdd_fail:
	regulator_disable(pw->avdd);

ov7695_avdd_fail:
	gpio_set_value(CAM_RSTN, 0);
	/* put CSIE IOs into DPD mode to save additional power for ardbeg */
	tegra_io_dpd_enable(&csie_io);
	return -ENODEV;
}

static int ardbeg_ov7695_power_off(struct ov7695_power_rail *pw)
{
	if (unlikely(WARN_ON(!pw || !pw->avdd || !pw->iovdd))) {
		/* put CSIE IOs into DPD mode to
		 * save additional power for ardbeg
		 */
		tegra_io_dpd_enable(&csie_io);
		return -EFAULT;
	}
	usleep_range(100, 120);

	gpio_set_value(CAM2_PWDN, 0);
	usleep_range(100, 120);

	regulator_disable(pw->iovdd);
	usleep_range(100, 120);

	regulator_disable(pw->avdd);

	/* put CSIE IOs into DPD mode to save additional power for ardbeg */
	tegra_io_dpd_enable(&csie_io);
	return 0;
}

struct ov7695_platform_data ardbeg_ov7695_pdata = {
	.power_on = ardbeg_ov7695_power_on,
	.power_off = ardbeg_ov7695_power_off,
	.mclk_name = "mclk2",
};

static int ardbeg_mt9m114_power_on(struct mt9m114_power_rail *pw)
{
	int err;
	if (unlikely(!pw || !pw->avdd || !pw->iovdd))
		return -EFAULT;

	/* disable CSIE IOs DPD mode to turn on front camera for ardbeg */
	tegra_io_dpd_disable(&csie_io);

	gpio_set_value(CAM_RSTN, 0);
	gpio_set_value(CAM2_PWDN, 1);
	usleep_range(1000, 1020);

	err = regulator_enable(pw->iovdd);
	if (unlikely(err))
		goto mt9m114_iovdd_fail;

	err = regulator_enable(pw->avdd);
	if (unlikely(err))
		goto mt9m114_avdd_fail;

	usleep_range(1000, 1020);
	gpio_set_value(CAM_RSTN, 1);
	gpio_set_value(CAM2_PWDN, 0);
	usleep_range(1000, 1020);

	/* return 1 to skip the in-driver power_on swquence */
	return 1;

mt9m114_avdd_fail:
	regulator_disable(pw->iovdd);

mt9m114_iovdd_fail:
	gpio_set_value(CAM_RSTN, 0);
	/* put CSIE IOs into DPD mode to save additional power for ardbeg */
	tegra_io_dpd_enable(&csie_io);
	return -ENODEV;
}

static int ardbeg_mt9m114_power_off(struct mt9m114_power_rail *pw)
{
	if (unlikely(!pw || !pw->avdd || !pw->iovdd)) {
		/* put CSIE IOs into DPD mode to
		 * save additional power for ardbeg
		 */
		tegra_io_dpd_enable(&csie_io);
		return -EFAULT;
	}

	usleep_range(100, 120);
	gpio_set_value(CAM_RSTN, 0);
	usleep_range(100, 120);
	regulator_disable(pw->avdd);
	usleep_range(100, 120);
	regulator_disable(pw->iovdd);

	/* put CSIE IOs into DPD mode to save additional power for ardbeg */
	tegra_io_dpd_enable(&csie_io);
	return 1;
}

struct mt9m114_platform_data ardbeg_mt9m114_pdata = {
	.power_on = ardbeg_mt9m114_power_on,
	.power_off = ardbeg_mt9m114_power_off,
	.mclk_name = "mclk2",
};


static int ardbeg_ov5693_power_on(struct ov5693_power_rail *pw)
{
	int err;

	pr_info("%s\n", __func__);
	if (unlikely(WARN_ON(!pw || !pw->dovdd || !pw->avdd)))
		return -EFAULT;

	/* disable CSIA/B IOs DPD mode to turn on camera for ardbeg */
	tegra_io_dpd_disable(&csie_io);

	if (ardbeg_get_extra_regulators())
		goto ov5693_poweron_fail;

	gpio_set_value(CAM2_PWDN, 0);
	gpio_set_value(CAM2_RSTN, 0);
	gpio_set_value(CAM_AF_PWDN, 0);
	usleep_range(10, 20);

	err = regulator_enable(ov5693_afvdd);
	if (err)
		goto ov5693_afvdd_fail;
	
	err = regulator_enable(pw->avdd);
	if (err)
		goto ov5693_avdd_fail;

	err = regulator_enable(ov5693_1v8);
	if (err)
		goto ov5693_1v8_fail;

	gpio_set_value(CAM2_PWDN, 1);
	
	err = regulator_enable(ov5693_1v2);
	if (err)
		goto ov5693_1v2_fail;
	
	udelay(2);
	gpio_set_value(CAM2_RSTN, 1);

	usleep_range(1000, 1110);

	pr_info("%s finished\n", __func__);
	return 0;

ov5693_afvdd_fail:
	regulator_disable(pw->avdd);

ov5693_avdd_fail:
	regulator_disable(ov5693_1v2);
	
ov5693_1v2_fail:
	regulator_disable(ov5693_1v8);

ov5693_1v8_fail:
	gpio_set_value(CAM2_PWDN, 0);
	gpio_set_value(CAM2_RSTN, 0);

ov5693_poweron_fail:
	/* put CSIA/B IOs into DPD mode to save additional power for ardbeg */
	tegra_io_dpd_enable(&csia_io);
	tegra_io_dpd_enable(&csib_io);
	pr_err("%s FAILED\n", __func__);
	return -ENODEV;
}

static int ardbeg_ov5693_power_off(struct ov5693_power_rail *pw)
{
	pr_info("%s\n", __func__);
	
	if (unlikely(WARN_ON(!pw || !pw->dovdd || !pw->avdd))) {
		tegra_io_dpd_enable(&csie_io);
		return -EFAULT;
	}

	usleep_range(21, 25);
	gpio_set_value(CAM2_RSTN, 0);
	udelay(2);

	regulator_disable(ov5693_1v2);
	regulator_disable(ov5693_1v8);
	gpio_set_value(CAM2_PWDN, 0);
	gpio_set_value(CAM_AF_PWDN, 0);
	regulator_disable(pw->avdd);
	regulator_disable(ov5693_afvdd);
	tegra_io_dpd_enable(&csie_io);

	/* put CSIA/B IOs into DPD mode to save additional power for ardbeg */
	tegra_io_dpd_enable(&csia_io);
	tegra_io_dpd_enable(&csib_io);
	pr_info("%s finished\n", __func__);
	return 0;
}

static struct nvc_gpio_pdata ov5693_gpio_pdata[] = {
	{ OV5693_GPIO_TYPE_PWRDN, CAM2_RSTN, true, 0, },
};


static struct ov5693_platform_data ardbeg_ov5693_pdata = {
	.gpio_count	= ARRAY_SIZE(ov5693_gpio_pdata),
	.gpio		= ov5693_gpio_pdata,
	.power_on	= ardbeg_ov5693_power_on,
	.power_off	= ardbeg_ov5693_power_off,
	.mclk_name	= "mclk2",
};

static int ardbeg_ad5823_power_on(struct ad5823_platform_data *pdata)
{
	int err = 0;

	pr_info("%s\n", __func__);
	gpio_set_value_cansleep(pdata->gpio, 1);
	pdata->pwr_dev = AD5823_PWR_DEV_ON;
	pr_info("%s finshed\n", __func__);

	return err;
}

static int ardbeg_ad5823_power_off(struct ad5823_platform_data *pdata)
{
	pr_info("%s\n", __func__);
	gpio_set_value_cansleep(pdata->gpio, 0);
	pdata->pwr_dev = AD5823_PWR_DEV_OFF;
	pr_info("%s finished\n", __func__);

	return 0;
}

static struct ad5823_platform_data ardbeg_ad5823_pdata = {
	.gpio = CAM_AF_PWDN,
	.power_on	= ardbeg_ad5823_power_on,
	.power_off	= ardbeg_ad5823_power_off,
};

static struct i2c_board_info	ardbeg_i2c_board_info_imx179 = {
	I2C_BOARD_INFO("imx179", 0x10),
	.platform_data = &ardbeg_imx179_data,
};

static struct i2c_board_info	ardbeg_i2c_board_info_ov5693 = {
	I2C_BOARD_INFO("ov5693", 0x36),
	.platform_data = &ardbeg_ov5693_pdata,
};

static struct i2c_board_info	ardbeg_i2c_board_info_ad5823 = {
	I2C_BOARD_INFO("ad5823", 0x0c),
	.platform_data = &ardbeg_ad5823_pdata,
};

static struct camera_module ardbeg_camera_module_info[] = {
	/* E1823 camera board */
	{
		/* rear camera */
		.sensor = &ardbeg_i2c_board_info_imx179,
		.focuser = &ardbeg_i2c_board_info_ad5823,
	},
	{
		/* front camera */
		.sensor = &ardbeg_i2c_board_info_ov5693,
	},
	{}
};

static struct camera_platform_data ardbeg_pcl_pdata = {
	.cfg = 0xAA55AA55,
	.modules = ardbeg_camera_module_info,
};

static struct platform_device ardbeg_camera_generic = {
	.name = "pcl-generic",
	.id = -1,
};

static int ardbeg_camera_init(void)
{
	struct board_info board_info;

	pr_debug("%s:++\n", __func__);
	tegra_get_board_info(&board_info);

	if (!of_machine_is_compatible("nvidia,tn8") &&
	    !of_machine_is_compatible("nvidia,mocha")) {
		/* put CSIA/B/E IOs into DPD mode to
		 * save additional power for ardbeg
		 */
		tegra_io_dpd_enable(&csia_io);
		tegra_io_dpd_enable(&csib_io);
		tegra_io_dpd_enable(&csie_io);
	}

	/* put CSIA/B/E IOs into DPD mode to
	 * save additional power for ardbeg
	 */
	tegra_io_dpd_enable(&csia_io);
	tegra_io_dpd_enable(&csib_io);
	tegra_io_dpd_enable(&csie_io);

	platform_device_add_data(&ardbeg_camera_generic,
		&ardbeg_pcl_pdata, sizeof(ardbeg_pcl_pdata));
	platform_device_register(&ardbeg_camera_generic);

#if IS_ENABLED(CONFIG_SOC_CAMERA_PLATFORM)
	platform_device_register(&ardbeg_soc_camera_device);
#endif

	return 0;
}

static struct pid_thermal_gov_params cpu_pid_params = {
	.max_err_temp = 4000,
	.max_err_gain = 1000,

	.gain_p = 1000,
	.gain_d = 0,

	.up_compensation = 15,
	.down_compensation = 15,
};

static struct thermal_zone_params cpu_tzp = {
	.governor_name = "pid_thermal_gov",
	.governor_params = &cpu_pid_params,
};

static struct thermal_zone_params board_tzp = {
	.governor_name = "step_wise"
};

static struct throttle_table cpu_throttle_table[] = {
	/* CPU_THROT_LOW cannot be used by other than CPU */
	/*      CPU,    GPU,  C2BUS,  C3BUS,   SCLK,    EMC   */
	{ { 2218500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 2193000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 2167500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 2142000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 2116500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 2091000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 2065500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 2040000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 2014500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1989000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1963500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1938000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1912500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1887000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1861500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1836000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1810500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1785000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1759500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1734000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1708500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1683000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1657500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1632000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1606500, 790000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1581000, 776000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1555500, 762000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1530000, 749000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1504500, 735000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1479000, 721000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1453500, 707000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1428000, 693000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1402500, 679000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1377000, 666000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1351500, 652000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1326000, 638000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1300500, 624000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1275000, 610000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1249500, 596000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1224000, 582000, NO_CAP, NO_CAP, NO_CAP, 792000 } },
	{ { 1198500, 569000, NO_CAP, NO_CAP, NO_CAP, 792000 } },
	{ { 1173000, 555000, NO_CAP, NO_CAP, 360000, 792000 } },
	{ { 1147500, 541000, NO_CAP, NO_CAP, 360000, 792000 } },
	{ { 1122000, 527000, NO_CAP, 684000, 360000, 792000 } },
	{ { 1096500, 513000, 444000, 684000, 360000, 792000 } },
	{ { 1071000, 499000, 444000, 684000, 360000, 792000 } },
	{ { 1045500, 486000, 444000, 684000, 360000, 792000 } },
	{ { 1020000, 472000, 444000, 684000, 324000, 792000 } },
	{ {  994500, 458000, 444000, 684000, 324000, 792000 } },
	{ {  969000, 444000, 444000, 600000, 324000, 792000 } },
	{ {  943500, 430000, 444000, 600000, 324000, 792000 } },
	{ {  918000, 416000, 396000, 600000, 324000, 792000 } },
	{ {  892500, 402000, 396000, 600000, 324000, 792000 } },
	{ {  867000, 389000, 396000, 600000, 324000, 792000 } },
	{ {  841500, 375000, 396000, 600000, 288000, 792000 } },
	{ {  816000, 361000, 396000, 600000, 288000, 792000 } },
	{ {  790500, 347000, 396000, 600000, 288000, 792000 } },
	{ {  765000, 333000, 396000, 504000, 288000, 792000 } },
	{ {  739500, 319000, 348000, 504000, 288000, 792000 } },
	{ {  714000, 306000, 348000, 504000, 288000, 624000 } },
	{ {  688500, 292000, 348000, 504000, 288000, 624000 } },
	{ {  663000, 278000, 348000, 504000, 288000, 624000 } },
	{ {  637500, 264000, 348000, 504000, 288000, 624000 } },
	{ {  612000, 250000, 348000, 504000, 252000, 624000 } },
	{ {  586500, 236000, 348000, 504000, 252000, 624000 } },
	{ {  561000, 222000, 348000, 420000, 252000, 624000 } },
	{ {  535500, 209000, 288000, 420000, 252000, 624000 } },
	{ {  510000, 195000, 288000, 420000, 252000, 624000 } },
	{ {  484500, 181000, 288000, 420000, 252000, 624000 } },
	{ {  459000, 167000, 288000, 420000, 252000, 624000 } },
	{ {  433500, 153000, 288000, 420000, 252000, 396000 } },
	{ {  408000, 139000, 288000, 420000, 252000, 396000 } },
	{ {  382500, 126000, 288000, 420000, 252000, 396000 } },
	{ {  357000, 112000, 288000, 420000, 252000, 396000 } },
	{ {  331500,  98000, 288000, 420000, 252000, 396000 } },
	{ {  306000,  84000, 288000, 420000, 252000, 396000 } },
	{ {  280500,  84000, 288000, 420000, 252000, 396000 } },
	{ {  255000,  84000, 288000, 420000, 252000, 396000 } },
	{ {  229500,  84000, 288000, 420000, 252000, 396000 } },
	{ {  204000,  84000, 288000, 420000, 252000, 396000 } },
};

static struct balanced_throttle cpu_throttle = {
	.throt_tab_size = ARRAY_SIZE(cpu_throttle_table),
	.throt_tab = cpu_throttle_table,
};

static struct throttle_table gpu_throttle_table[] = {
	/* CPU_THROT_LOW cannot be used by other than CPU */
	/*      CPU,    GPU,  C2BUS,  C3BUS,   SCLK,    EMC   */
	{ { 2218500, 782800, 480000, 756000, 384000, 924000 } },
	{ { 2218500, 772200, 480000, 756000, 384000, 924000 } },
	{ { 2218000, 761600, 480000, 756000, 384000, 924000 } },
	{ { 2218500, 751100, 480000, 756000, 384000, 924000 } },
	{ { 2193000, 740500, 480000, 756000, 384000, 924000 } },
	{ { 2167500, 729900, 480000, 756000, 384000, 924000 } },
	{ { 2142000, 719300, 480000, 756000, 384000, 924000 } },
	{ { 2116500, 708700, 480000, 756000, 384000, 924000 } },
	{ { 2091000, 698100, 480000, 756000, 384000, 924000 } },
	{ { 2065500, 687500, 480000, 756000, 384000, 924000 } },
	{ { 2040000, 676900, 480000, 756000, 384000, 924000 } },
	{ { 2014500, 666000, 480000, 756000, 384000, 924000 } },
	{ { 1989000, 656000, 480000, 756000, 384000, 924000 } },
	{ { 1963500, 645000, 480000, 756000, 384000, 924000 } },
	{ { 1938000, 635000, 480000, 756000, 384000, 924000 } },
	{ { 1912500, 624000, 480000, 756000, 384000, 924000 } },
	{ { 1887000, 613000, 480000, 756000, 384000, 924000 } },
	{ { 1861500, 603000, 480000, 756000, 384000, 924000 } },
	{ { 1836000, 592000, 480000, 756000, 384000, 924000 } },
	{ { 1810500, 582000, 480000, 756000, 384000, 924000 } },
	{ { 1785000, 571000, 480000, 756000, 384000, 924000 } },
	{ { 1759500, 560000, 480000, 756000, 384000, 924000 } },
	{ { 1734000, 550000, 480000, 756000, 384000, 924000 } },
	{ { 1708500, 539000, 480000, 756000, 384000, 924000 } },
	{ { 1683000, 529000, 480000, 756000, 384000, 924000 } },
	{ { 1657500, 518000, 480000, 756000, 384000, 924000 } },
	{ { 1632000, 508000, 480000, 756000, 384000, 924000 } },
	{ { 1606500, 497000, 480000, 756000, 384000, 924000 } },
	{ { 1581000, 486000, 480000, 756000, 384000, 924000 } },
	{ { 1555500, 476000, 480000, 756000, 384000, 924000 } },
	{ { 1530000, 465000, 480000, 756000, 384000, 924000 } },
	{ { 1504500, 455000, 480000, 756000, 384000, 924000 } },
	{ { 1479000, 444000, 480000, 756000, 384000, 924000 } },
	{ { 1453500, 433000, 480000, 756000, 384000, 924000 } },
	{ { 1428000, 423000, 480000, 756000, 384000, 924000 } },
	{ { 1402500, 412000, 480000, 756000, 384000, 924000 } },
	{ { 1377000, 402000, 480000, 756000, 384000, 924000 } },
	{ { 1351500, 391000, 480000, 756000, 384000, 924000 } },
	{ { 1326000, 380000, 480000, 756000, 384000, 924000 } },
	{ { 1300500, 370000, 480000, 756000, 384000, 924000 } },
	{ { 1275000, 359000, 480000, 756000, 384000, 924000 } },
	{ { 1249500, 349000, 480000, 756000, 384000, 924000 } },
	{ { 1224000, 338000, 480000, 756000, 384000, 792000 } },
	{ { 1198500, 328000, 480000, 756000, 384000, 792000 } },
	{ { 1173000, 317000, 480000, 756000, 360000, 792000 } },
	{ { 1147500, 306000, 480000, 756000, 360000, 792000 } },
	{ { 1122000, 296000, 480000, 684000, 360000, 792000 } },
	{ { 1096500, 285000, 444000, 684000, 360000, 792000 } },
	{ { 1071000, 275000, 444000, 684000, 360000, 792000 } },
	{ { 1045500, 264000, 444000, 684000, 360000, 792000 } },
	{ { 1020000, 253000, 444000, 684000, 324000, 792000 } },
	{ {  994500, 243000, 444000, 684000, 324000, 792000 } },
	{ {  969000, 232000, 444000, 600000, 324000, 792000 } },
	{ {  943500, 222000, 444000, 600000, 324000, 792000 } },
	{ {  918000, 211000, 396000, 600000, 324000, 792000 } },
	{ {  892500, 200000, 396000, 600000, 324000, 792000 } },
	{ {  867000, 190000, 396000, 600000, 324000, 792000 } },
	{ {  841500, 179000, 396000, 600000, 288000, 792000 } },
	{ {  816000, 169000, 396000, 600000, 288000, 792000 } },
	{ {  790500, 158000, 396000, 600000, 288000, 792000 } },
	{ {  765000, 148000, 396000, 504000, 288000, 792000 } },
	{ {  739500, 137000, 348000, 504000, 288000, 792000 } },
	{ {  714000, 126000, 348000, 504000, 288000, 624000 } },
	{ {  688500, 116000, 348000, 504000, 288000, 624000 } },
	{ {  663000, 105000, 348000, 504000, 288000, 624000 } },
	{ {  637500,  95000, 348000, 504000, 288000, 624000 } },
	{ {  612000,  84000, 348000, 504000, 252000, 624000 } },
	{ {  586500,  84000, 348000, 504000, 252000, 624000 } },
	{ {  561000,  84000, 348000, 420000, 252000, 624000 } },
	{ {  535500,  84000, 288000, 420000, 252000, 624000 } },
	{ {  510000,  84000, 288000, 420000, 252000, 624000 } },
	{ {  484500,  84000, 288000, 420000, 252000, 624000 } },
	{ {  459000,  84000, 288000, 420000, 252000, 624000 } },
	{ {  433500,  84000, 288000, 420000, 252000, 396000 } },
	{ {  408000,  84000, 288000, 420000, 252000, 396000 } },
	{ {  382500,  84000, 288000, 420000, 252000, 396000 } },
	{ {  357000,  84000, 288000, 420000, 252000, 396000 } },
	{ {  331500,  84000, 288000, 420000, 252000, 396000 } },
	{ {  306000,  84000, 288000, 420000, 252000, 396000 } },
	{ {  280500,  84000, 288000, 420000, 252000, 396000 } },
	{ {  255000,  84000, 288000, 420000, 252000, 396000 } },
	{ {  229500,  84000, 288000, 420000, 252000, 396000 } },
	{ {  204000,  84000, 288000, 420000, 252000, 396000 } },
};

static struct balanced_throttle gpu_throttle = {
	.throt_tab_size = ARRAY_SIZE(gpu_throttle_table),
	.throt_tab = gpu_throttle_table,
};

static int __init ardbeg_tj_throttle_init(void)
{
	void *r1, *r2;

	if (of_machine_is_compatible("nvidia,ardbeg") ||
	    of_machine_is_compatible("nvidia,mocha") ||
	    of_machine_is_compatible("nvidia,norrin") ||
	    of_machine_is_compatible("nvidia,bowmore") ||
	    of_machine_is_compatible("nvidia,tn8")) {
		r1 = balanced_throttle_register(&cpu_throttle, "cpu-balanced");
		r2 = balanced_throttle_register(&gpu_throttle, "gpu-balanced");
		if (!r1 || !r2)
			pr_err("%s: balanced_throttle_register FAILED.\n",
				__func__);
	}

	return 0;
}
late_initcall(ardbeg_tj_throttle_init);

#ifdef CONFIG_TEGRA_SKIN_THROTTLE
static struct thermal_trip_info skin_trips[] = {
	{
		.cdev_type = "skin-balanced",
		.trip_temp = 47000,
		.trip_type = THERMAL_TRIP_PASSIVE,
		.upper = THERMAL_NO_LIMIT,
		.lower = THERMAL_NO_LIMIT,
		.hysteresis = 0,
	}
};

static struct therm_est_subdevice skin_devs[] = {
	{
		.dev_data = "Tdiode_tegra",
		.coeffs = {
			2, 2, 1, 1,
			1, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 2, 3
		},
	},
	{
		.dev_data = "Tboard_tegra",
		.coeffs = {
			-5, 1, 0, 1,
			1, 0, 1, 1,
			0, 2, -1, 1,
			2, 3, 3, 2,
			2, 1, 8, 53
		},
	},
};

static struct therm_est_subdevice tn8ffd_skin_devs[] = {
	{
		.dev_data = "Tdiode",
		.coeffs = {
			3, 0, 0, 0,
			1, 0, -1, 0,
			1, 0, 0, 1,
			1, 0, 0, 0,
			0, 1, 2, 2
		},
	},
	{
		.dev_data = "Tboard",
		.coeffs = {
			1, 1, 2, 8,
			6, -8, -13, -9,
			-9, -8, -17, -18,
			-18, -16, 2, 17,
			15, 27, 42, 60
		},
	},
};

static struct pid_thermal_gov_params skin_pid_params = {
	.max_err_temp = 4000,
	.max_err_gain = 1000,

	.gain_p = 1000,
	.gain_d = 0,

	.up_compensation = 15,
	.down_compensation = 15,
};

static struct thermal_zone_params skin_tzp = {
	.governor_name = "pid_thermal_gov",
	.governor_params = &skin_pid_params,
};

static struct therm_est_data skin_data = {
	.num_trips = ARRAY_SIZE(skin_trips),
	.trips = skin_trips,
	.polling_period = 1100,
	.passive_delay = 15000,
	.tc1 = 10,
	.tc2 = 1,
	.tzp = &skin_tzp,
	.use_activator = 0,
};

static struct throttle_table skin_throttle_table[] = {
	/* CPU_THROT_LOW cannot be used by other than CPU */
	/*      CPU,    GPU,  C2BUS,  C3BUS,   SCLK,    EMC   */
	{ { 2218500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 2193000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 2167500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 2142000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 2116500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 2091000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 2065500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 2040000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 2014500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1989000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1963500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1938000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1912500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1887000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1861500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1836000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1810500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1785000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1759500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1734000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1708500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1683000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1657500, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1632000, NO_CAP, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1606500, 790000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1581000, 776000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1555500, 762000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1530000, 749000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1504500, 735000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1479000, 721000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1453500, 707000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1428000, 693000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1402500, 679000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1377000, 666000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1351500, 652000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1326000, 638000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1300500, 624000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1275000, 610000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1249500, 596000, NO_CAP, NO_CAP, NO_CAP, NO_CAP } },
	{ { 1224000, 582000, NO_CAP, NO_CAP, NO_CAP, 792000 } },
	{ { 1198500, 569000, NO_CAP, NO_CAP, NO_CAP, 792000 } },
	{ { 1173000, 555000, NO_CAP, NO_CAP, 360000, 792000 } },
	{ { 1147500, 541000, NO_CAP, NO_CAP, 360000, 792000 } },
	{ { 1122000, 527000, NO_CAP, 684000, 360000, 792000 } },
	{ { 1096500, 513000, 444000, 684000, 360000, 792000 } },
	{ { 1071000, 499000, 444000, 684000, 360000, 792000 } },
	{ { 1045500, 486000, 444000, 684000, 360000, 792000 } },
	{ { 1020000, 472000, 444000, 684000, 324000, 792000 } },
	{ {  994500, 458000, 444000, 684000, 324000, 792000 } },
	{ {  969000, 444000, 444000, 600000, 324000, 792000 } },
	{ {  943500, 430000, 444000, 600000, 324000, 792000 } },
	{ {  918000, 416000, 396000, 600000, 324000, 792000 } },
	{ {  892500, 402000, 396000, 600000, 324000, 792000 } },
	{ {  867000, 389000, 396000, 600000, 324000, 792000 } },
	{ {  841500, 375000, 396000, 600000, 288000, 792000 } },
	{ {  816000, 361000, 396000, 600000, 288000, 792000 } },
	{ {  790500, 347000, 396000, 600000, 288000, 792000 } },
	{ {  765000, 333000, 396000, 504000, 288000, 792000 } },
	{ {  739500, 319000, 348000, 504000, 288000, 792000 } },
	{ {  714000, 306000, 348000, 504000, 288000, 624000 } },
	{ {  688500, 292000, 348000, 504000, 288000, 624000 } },
	{ {  663000, 278000, 348000, 504000, 288000, 624000 } },
	{ {  637500, 264000, 348000, 504000, 288000, 624000 } },
	{ {  612000, 250000, 348000, 504000, 252000, 624000 } },
	{ {  586500, 236000, 348000, 504000, 252000, 624000 } },
	{ {  561000, 222000, 348000, 420000, 252000, 624000 } },
	{ {  535500, 209000, 288000, 420000, 252000, 624000 } },
	{ {  510000, 195000, 288000, 420000, 252000, 624000 } },
	{ {  484500, 181000, 288000, 420000, 252000, 624000 } },
	{ {  459000, 167000, 288000, 420000, 252000, 624000 } },
	{ {  433500, 153000, 288000, 420000, 252000, 396000 } },
	{ {  408000, 139000, 288000, 420000, 252000, 396000 } },
	{ {  382500, 126000, 288000, 420000, 252000, 396000 } },
	{ {  357000, 112000, 288000, 420000, 252000, 396000 } },
	{ {  331500,  98000, 288000, 420000, 252000, 396000 } },
	{ {  306000,  84000, 288000, 420000, 252000, 396000 } },
	{ {  280500,  84000, 288000, 420000, 252000, 396000 } },
	{ {  255000,  84000, 288000, 420000, 252000, 396000 } },
	{ {  229500,  84000, 288000, 420000, 252000, 396000 } },
	{ {  204000,  84000, 288000, 420000, 252000, 396000 } },
};

static struct balanced_throttle skin_throttle = {
	.throt_tab_size = ARRAY_SIZE(skin_throttle_table),
	.throt_tab = skin_throttle_table,
};

static int __init ardbeg_skin_init(void)
{
	struct board_info board_info;

	tegra_get_board_info(&board_info);

	if (of_machine_is_compatible("nvidia,ardbeg") ||
		of_machine_is_compatible("nvidia,mocha") ||
		of_machine_is_compatible("nvidia,tn8")) {
		if (board_info.board_id == BOARD_P1761 ||
			board_info.board_id == BOARD_E1784 ||
			board_info.board_id == BOARD_E1922) {
			skin_data.ndevs = ARRAY_SIZE(tn8ffd_skin_devs);
			skin_data.devs = tn8ffd_skin_devs;
			skin_data.toffset = 4034;
		} else {
			skin_data.ndevs = ARRAY_SIZE(skin_devs);
			skin_data.devs = skin_devs;
			skin_data.toffset = 3543;
		}

		balanced_throttle_register(&skin_throttle, "skin-balanced");
		tegra_skin_therm_est_device.dev.platform_data = &skin_data;
		platform_device_register(&tegra_skin_therm_est_device);
	}
	return 0;
}
late_initcall(ardbeg_skin_init);
#endif

static struct nct1008_platform_data ardbeg_nct72_pdata = {
	.loc_name = "tegra",
	.supported_hwrev = true,
	.conv_rate = 0x06, /* 4Hz conversion rate */
	.offset = 0,
	.extended_range = true,

	.sensors = {
		[LOC] = {
			.tzp = &board_tzp,
			.shutdown_limit = 120, /* C */
			.passive_delay = 1000,
			.num_trips = 1,
			.trips = {
				{
					.cdev_type = "therm_est_activ",
					.trip_temp = 40000,
					.trip_type = THERMAL_TRIP_ACTIVE,
					.hysteresis = 1000,
					.upper = THERMAL_NO_LIMIT,
					.lower = THERMAL_NO_LIMIT,
					.mask = 1,
				},
			},
		},
		[EXT] = {
			.tzp = &cpu_tzp,
			.shutdown_limit = 95, /* C */
			.passive_delay = 1000,
			.num_trips = 2,
			.trips = {
				{
					.cdev_type = "shutdown_warning",
					.trip_temp = 93000,
					.trip_type = THERMAL_TRIP_PASSIVE,
					.upper = THERMAL_NO_LIMIT,
					.lower = THERMAL_NO_LIMIT,
					.mask = 0,
				},
				{
					.cdev_type = "cpu-balanced",
					.trip_temp = 83000,
					.trip_type = THERMAL_TRIP_PASSIVE,
					.upper = THERMAL_NO_LIMIT,
					.lower = THERMAL_NO_LIMIT,
					.hysteresis = 1000,
					.mask = 1,
				},
			}
		}
	}
};

#ifdef CONFIG_TEGRA_SKIN_THROTTLE
static struct nct1008_platform_data ardbeg_nct72_tskin_pdata = {
	.loc_name = "skin",

	.supported_hwrev = true,
	.conv_rate = 0x06, /* 4Hz conversion rate */
	.offset = 0,
	.extended_range = true,

	.sensors = {
		[LOC] = {
			.shutdown_limit = 95, /* C */
			.num_trips = 0,
			.tzp = NULL,
		},
		[EXT] = {
			.shutdown_limit = 85, /* C */
			.passive_delay = 10000,
			.polling_delay = 1000,
			.tzp = &skin_tzp,
			.num_trips = 1,
			.trips = {
				{
					.cdev_type = "skin-balanced",
					.trip_temp = 50000,
					.trip_type = THERMAL_TRIP_PASSIVE,
					.upper = THERMAL_NO_LIMIT,
					.lower = THERMAL_NO_LIMIT,
					.mask = 1,
				},
			},
		}
	}
};
#endif

static struct i2c_board_info ardbeg_i2c_nct72_board_info[] = {
	{
		I2C_BOARD_INFO("nct72", 0x4c),
		.platform_data = &ardbeg_nct72_pdata,
		.irq = -1,
	},
#ifdef CONFIG_TEGRA_SKIN_THROTTLE
	{
		I2C_BOARD_INFO("nct72", 0x4d),
		.platform_data = &ardbeg_nct72_tskin_pdata,
		.irq = -1,
	}
#endif
};

static struct i2c_board_info laguna_i2c_nct72_board_info[] = {
	{
		I2C_BOARD_INFO("nct72", 0x4c),
		.platform_data = &ardbeg_nct72_pdata,
		.irq = -1,
	},
};

static int ardbeg_nct72_init(void)
{
	s32 base_cp, shft_cp;
	u32 base_ft, shft_ft;
	int nct72_port = TEGRA_GPIO_PI6;
	int ret = 0;
	int i;
	struct thermal_trip_info *trip_state;
	struct board_info board_info;

	tegra_get_board_info(&board_info);
	/* raise NCT's thresholds if soctherm CP,FT fuses are ok */
	if ((tegra_fuse_calib_base_get_cp(&base_cp, &shft_cp) >= 0) &&
	    (tegra_fuse_calib_base_get_ft(&base_ft, &shft_ft) >= 0)) {
		ardbeg_nct72_pdata.sensors[EXT].shutdown_limit += 20;
		for (i = 0; i < ardbeg_nct72_pdata.sensors[EXT].num_trips;
			 i++) {
			trip_state = &ardbeg_nct72_pdata.sensors[EXT].trips[i];
			if (!strncmp(trip_state->cdev_type, "cpu-balanced",
					THERMAL_NAME_LENGTH)) {
				trip_state->cdev_type = "_none_";
				break;
			}
		}
	} else {
		tegra_platform_edp_init(
			ardbeg_nct72_pdata.sensors[EXT].trips,
			&ardbeg_nct72_pdata.sensors[EXT].num_trips,
					12000); /* edp temperature margin */
		tegra_add_cpu_vmax_trips(
			ardbeg_nct72_pdata.sensors[EXT].trips,
			&ardbeg_nct72_pdata.sensors[EXT].num_trips);
		tegra_add_tgpu_trips(
			ardbeg_nct72_pdata.sensors[EXT].trips,
			&ardbeg_nct72_pdata.sensors[EXT].num_trips);
		tegra_add_vc_trips(
			ardbeg_nct72_pdata.sensors[EXT].trips,
			&ardbeg_nct72_pdata.sensors[EXT].num_trips);
		tegra_add_core_vmax_trips(
			ardbeg_nct72_pdata.sensors[EXT].trips,
			&ardbeg_nct72_pdata.sensors[EXT].num_trips);
	}

	/* vmin trips are bound to soctherm on norrin and bowmore */
	if (!(board_info.board_id == BOARD_PM374 ||
		board_info.board_id == BOARD_E1971 ||
		board_info.board_id == BOARD_E1991))
		tegra_add_all_vmin_trips(ardbeg_nct72_pdata.sensors[EXT].trips,
			&ardbeg_nct72_pdata.sensors[EXT].num_trips);

	ardbeg_i2c_nct72_board_info[0].irq = gpio_to_irq(nct72_port);

	ret = gpio_request(nct72_port, "temp_alert");
	if (ret < 0)
		return ret;

	ret = gpio_direction_input(nct72_port);
	if (ret < 0) {
		pr_info("%s: calling gpio_free(nct72_port)", __func__);
		gpio_free(nct72_port);
	}

	/* norrin has thermal sensor on GEN1-I2C i.e. instance 0 */
	if (board_info.board_id == BOARD_PM374)
		i2c_register_board_info(0, ardbeg_i2c_nct72_board_info,
					1); /* only register device[0] */
	/* ardbeg has thermal sensor on GEN2-I2C i.e. instance 1 */
	else if (board_info.board_id == BOARD_PM358 ||
			board_info.board_id == BOARD_PM359 ||
			board_info.board_id == BOARD_PM370 ||
			board_info.board_id == BOARD_PM374 ||
			board_info.board_id == BOARD_PM363)
		i2c_register_board_info(1, laguna_i2c_nct72_board_info,
			ARRAY_SIZE(laguna_i2c_nct72_board_info));
	else if (board_info.board_id == BOARD_E1971 ||
		 board_info.board_id == BOARD_E1991)
		/* bowmore has thermal sensor on GEN1-I2C i.e. instance 0 */
		i2c_register_board_info(0, ardbeg_i2c_nct72_board_info,
					1); /* only register device[0] */
	else
		i2c_register_board_info(0, ardbeg_i2c_nct72_board_info,
			ARRAY_SIZE(ardbeg_i2c_nct72_board_info));

	return ret;
}

struct ntc_thermistor_adc_table {
	int temp; /* degree C */
	int adc;
};

static struct ntc_thermistor_adc_table tn8_thermistor_table[] = {
	{ -40, 2578 }, { -39, 2577 }, { -38, 2576 }, { -37, 2575 },
	{ -36, 2574 }, { -35, 2573 }, { -34, 2572 }, { -33, 2571 },
	{ -32, 2569 }, { -31, 2568 }, { -30, 2567 }, { -29, 2565 },
	{ -28, 2563 }, { -27, 2561 }, { -26, 2559 }, { -25, 2557 },
	{ -24, 2555 }, { -23, 2553 }, { -22, 2550 }, { -21, 2548 },
	{ -20, 2545 }, { -19, 2542 }, { -18, 2539 }, { -17, 2536 },
	{ -16, 2532 }, { -15, 2529 }, { -14, 2525 }, { -13, 2521 },
	{ -12, 2517 }, { -11, 2512 }, { -10, 2507 }, {  -9, 2502 },
	{  -8, 2497 }, {  -7, 2492 }, {  -6, 2486 }, {  -5, 2480 },
	{  -4, 2473 }, {  -3, 2467 }, {  -2, 2460 }, {  -1, 2452 },
	{   0, 2445 }, {   1, 2437 }, {   2, 2428 }, {   3, 2419 },
	{   4, 2410 }, {   5, 2401 }, {   6, 2391 }, {   7, 2380 },
	{   8, 2369 }, {   9, 2358 }, {  10, 2346 }, {  11, 2334 },
	{  12, 2322 }, {  13, 2308 }, {  14, 2295 }, {  15, 2281 },
	{  16, 2266 }, {  17, 2251 }, {  18, 2236 }, {  19, 2219 },
	{  20, 2203 }, {  21, 2186 }, {  22, 2168 }, {  23, 2150 },
	{  24, 2131 }, {  25, 2112 }, {  26, 2092 }, {  27, 2072 },
	{  28, 2052 }, {  29, 2030 }, {  30, 2009 }, {  31, 1987 },
	{  32, 1964 }, {  33, 1941 }, {  34, 1918 }, {  35, 1894 },
	{  36, 1870 }, {  37, 1845 }, {  38, 1820 }, {  39, 1795 },
	{  40, 1769 }, {  41, 1743 }, {  42, 1717 }, {  43, 1691 },
	{  44, 1664 }, {  45, 1637 }, {  46, 1610 }, {  47, 1583 },
	{  48, 1555 }, {  49, 1528 }, {  50, 1500 }, {  51, 1472 },
	{  52, 1445 }, {  53, 1417 }, {  54, 1390 }, {  55, 1362 },
	{  56, 1334 }, {  57, 1307 }, {  58, 1280 }, {  59, 1253 },
	{  60, 1226 }, {  61, 1199 }, {  62, 1172 }, {  63, 1146 },
	{  64, 1120 }, {  65, 1094 }, {  66, 1069 }, {  67, 1044 },
	{  68, 1019 }, {  69,  994 }, {  70,  970 }, {  71,  946 },
	{  72,  922 }, {  73,  899 }, {  74,  877 }, {  75,  854 },
	{  76,  832 }, {  77,  811 }, {  78,  789 }, {  79,  769 },
	{  80,  748 }, {  81,  729 }, {  82,  709 }, {  83,  690 },
	{  84,  671 }, {  85,  653 }, {  86,  635 }, {  87,  618 },
	{  88,  601 }, {  89,  584 }, {  90,  568 }, {  91,  552 },
	{  92,  537 }, {  93,  522 }, {  94,  507 }, {  95,  493 },
	{  96,  479 }, {  97,  465 }, {  98,  452 }, {  99,  439 },
	{ 100,  427 }, { 101,  415 }, { 102,  403 }, { 103,  391 },
	{ 104,  380 }, { 105,  369 }, { 106,  359 }, { 107,  349 },
	{ 108,  339 }, { 109,  329 }, { 110,  320 }, { 111,  310 },
	{ 112,  302 }, { 113,  293 }, { 114,  285 }, { 115,  277 },
	{ 116,  269 }, { 117,  261 }, { 118,  254 }, { 119,  247 },
	{ 120,  240 }, { 121,  233 }, { 122,  226 }, { 123,  220 },
	{ 124,  214 }, { 125,  208 },
};

static struct ntc_thermistor_adc_table *thermistor_table;
static int thermistor_table_size;

static int gadc_thermal_thermistor_adc_to_temp(
		struct gadc_thermal_platform_data *pdata, int val, int val2)
{
	int temp = 0, adc_hi, adc_lo;
	int i;

	for (i = 0; i < thermistor_table_size; i++)
		if (val >= thermistor_table[i].adc)
			break;

	if (i == 0) {
		temp = thermistor_table[i].temp * 1000;
	} else if (i >= (thermistor_table_size - 1)) {
		temp = thermistor_table[thermistor_table_size - 1].temp * 1000;
	} else {
		adc_hi = thermistor_table[i - 1].adc;
		adc_lo = thermistor_table[i].adc;
		temp = thermistor_table[i].temp * 1000;
		temp -= ((val - adc_lo) * 1000 / (adc_hi - adc_lo));
	}

	return temp;
};

#define TDIODE_PRECISION_MULTIPLIER	1000000000LL
#define TDIODE_MIN_TEMP			-25000LL
#define TDIODE_MAX_TEMP			125000LL

static int gadc_thermal_tdiode_adc_to_temp(
		struct gadc_thermal_platform_data *pdata, int val, int val2)
{
	/*
	 * Series resistance cancellation using multi-current ADC measurement.
	 * diode temp = ((adc2 - k * adc1) - (b2 - k * b1)) / (m2 - k * m1)
	 * - adc1 : ADC raw with current source 400uA
	 * - m1, b1 : calculated with current source 400uA
	 * - adc2 : ADC raw with current source 800uA
	 * - m2, b2 : calculated with current source 800uA
	 * - k : 2 (= 800uA / 400uA)
	 */
	const s64 m1 = -0.00571005 * TDIODE_PRECISION_MULTIPLIER;
	const s64 b1 = 2524.29891 * TDIODE_PRECISION_MULTIPLIER;
	const s64 m2 = -0.005519811 * TDIODE_PRECISION_MULTIPLIER;
	const s64 b2 = 2579.354349 * TDIODE_PRECISION_MULTIPLIER;
	s64 temp = TDIODE_PRECISION_MULTIPLIER;

	temp *= (s64)((val2) - 2 * (val));
	temp -= (b2 - 2 * b1);
	temp = div64_s64(temp, (m2 - 2 * m1));
	temp = min_t(s64, max_t(s64, temp, TDIODE_MIN_TEMP), TDIODE_MAX_TEMP);
	return temp;
};

static struct gadc_thermal_platform_data gadc_thermal_thermistor_pdata = {
	.iio_channel_name = "thermistor",
	.tz_name = "Tboard",
	.temp_offset = 0,
	.adc_to_temp = gadc_thermal_thermistor_adc_to_temp,

	.polling_delay = 15000,
	.num_trips = 1,
	.trips = {
		{
			.cdev_type = "therm_est_activ",
			.trip_temp = 40000,
			.trip_type = THERMAL_TRIP_ACTIVE,
			.hysteresis = 1000,
			.upper = THERMAL_NO_LIMIT,
			.lower = THERMAL_NO_LIMIT,
			.mask = 1,
		},
	},
	.tzp = &board_tzp,
};

static struct gadc_thermal_platform_data gadc_thermal_tdiode_pdata = {
	.iio_channel_name = "tdiode",
	.tz_name = "Tdiode",
	.temp_offset = 0,
	.dual_mode = true,
	.adc_to_temp = gadc_thermal_tdiode_adc_to_temp,
};

static struct platform_device gadc_thermal_thermistor = {
	.name   = "generic-adc-thermal",
	.id     = 1,
	.dev	= {
		.platform_data = &gadc_thermal_thermistor_pdata,
	},
};

static struct platform_device gadc_thermal_tdiode = {
	.name   = "generic-adc-thermal",
	.id     = 2,
	.dev	= {
		.platform_data = &gadc_thermal_tdiode_pdata,
	},
};

static struct platform_device *gadc_thermal_devices[] = {
	&gadc_thermal_thermistor,
	&gadc_thermal_tdiode,
};

int __init ardbeg_sensors_init(void)
{
	struct board_info board_info;
	tegra_get_board_info(&board_info);

	ardbeg_camera_init();

	if (board_info.board_id == BOARD_P1761 ||
		board_info.board_id == BOARD_E1784 ||
		board_info.board_id == BOARD_E1922) {
		platform_add_devices(gadc_thermal_devices,
				ARRAY_SIZE(gadc_thermal_devices));
		thermistor_table = &tn8_thermistor_table[0];
		thermistor_table_size = ARRAY_SIZE(tn8_thermistor_table);
	} else
		ardbeg_nct72_init();

#if defined(ARCH_TEGRA_12x_SOC)
	/* TN8 and PM359 don't have ALS CM32181 */
	if (!of_machine_is_compatible("nvidia,tn8") &&
	    board_info.board_id != BOARD_PM359 &&
		!of_machine_is_compatible("nvidia,mocha"))
		i2c_register_board_info(0, ardbeg_i2c_board_info_cm32181,
			ARRAY_SIZE(ardbeg_i2c_board_info_cm32181));
#endif
	return 0;
}

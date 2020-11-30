/*
 * automatically generated, don't change
 */

#include "qemu/osdep.h"
#include "qemu/log.h"
#include "hw/sysbus.h"
#include "hw/boards.h"
#include "qemu/units.h"
//#include "hw/arm/arm.h"
#include "exec/address-spaces.h"
#include "target/arm/cpu-qom.h"
#include "target/arm/cpu.h"
#include "hw/cpu/arm11mpcore.h"
#include "hw/char/serial.h"

#define TYPE_BEAGLEBOARD "beagleboard"
#define BEAGLEBOARD(obj) \
    OBJECT_CHECK(BeagleBoardState, (obj), TYPE_BEAGLEBOARD)

typedef struct BEAGLEBOARDState {
    ARMCPU *cpu;
    ARM11MPCorePriveState arm_arm11mp_gic;
    // timer
    MemoryRegion ram0;
} BEAGLEBOARDState;


static void beaglebaord_init(MachineState *machine)
{
    BEAGLEBOARDState *s = g_new0(BEAGLEBOARDState, 1);
    Error *err = NULL;
    //static struct arm_boot_info binfo;
    
    s->cpu = ARM_CPU(object_new(machine->cpu_type));
    object_property_set_bool(OBJECT(s->cpu), "realized", true, &err);
    
    //memory_region_allocate_system_memory(&s->ram0, OBJECT(machine), "ram0", 0x10000000);
    memory_region_add_subregion_overlap(get_system_memory(), 0x0, &s->ram0, 0);
    
    object_initialize(&s->arm_arm11mp_gic, sizeof(s->arm_arm11mp_gic), TYPE_ARM11MPCORE_PRIV);
    object_property_set_bool(OBJECT(&s->arm_arm11mp_gic), "realized", true, &err);
    sysbus_mmio_map(SYS_BUS_DEVICE(&s->arm_arm11mp_gic), 0, 0x47001000 & 0xffff0000);
    
    sysbus_connect_irq(SYS_BUS_DEVICE(&s->arm_arm11mp_gic), 0, qdev_get_gpio_in(DEVICE(s->cpu), ARM_CPU_IRQ));
    
   
    
    //serial_mm_init(get_system_memory(), 0x44200000, 0, qdev_get_gpio_in(DEVICE(&s->arm_arm11mp_gic), 23), 115200, serial_hd(0), DEVICE_LITTLE_ENDIAN);
    
    
    
    
    //plxtech_nas7820_plxtech_nas782x_reset(s);

    /*
    binfo.board_id = 0xFFFFFFFF;
    binfo.ram_size = machine->ram_size;
    binfo.kernel_filename = machine->kernel_filename;
    binfo.kernel_cmdline = machine->kernel_cmdline;
    binfo.initrd_filename = machine->initrd_filename;
    arm_load_kernel(ARM_CPU(first_cpu), &binfo);
    */
}

static void beagleboard_machine_init(MachineClass *mc)
{
    mc->desc = "beagle";
    mc->init = beaglebaord_init;
    mc->default_ram_size = 512 * MiB;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("arm11mpcore");
    mc->ignore_memory_transaction_failures = false;
}

DEFINE_MACHINE("beagle", beagleboard_machine_init)

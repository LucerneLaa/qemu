













#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/cutils.h"
#include "qapi/error.h"
#include "cpu.h"
#include "hw/arm/bcm2836.h"
#include "hw/registerfields.h"
#include "qemu/error-report.h"
#include "hw/boards.h"
#include "hw/loader.h"
#include "hw/arm/boot.h"
#include "sysemu/sysemu.h"
#include "qom/object.h"











struct BeagleMachineState {
    /*< private >*/
    MachineState parent_obj;
    /*< public >*/
    OMAP3530State soc;
    struct arm_boot_info binfo;
};
typedef struct BeagleMachineState BeagleMachineState;

struct BeagleMachineClass {
    /*< private >*/
    MachineClass parent_obj;
    /*< public >*/
    uint32_t board_rev;
};
typedef struct BeagleMachineClass BeagleMachineClass;


#define TYPE_BEAGLE_MACHINE     MACHINE_TYPE_NAME("beagle-common")
DECLARE_OBJ_CHECKERS(BeagleMachineState, BeagleMachineClass,
                     BEAGLE_MACHINE, TYPE_BEAGLE_MACHINE)

static void bealge_machine_init(MachineState *machine) 
{
    BeagleMachineClass *mc = BEAGLE_MACHINE_GET_CLASS(machine);
    BeagleMachineState *s = BEAGLE_MACHINE(machine);
    uint32_t board_rev = mc->board_rev;
    uint64_t ram_size = board_ram_size(baord_rev);
    uint32_t vcram_size;
    DriveInfo *di;
    BlockBackend *blk;
    BusState *bus;
    DeviceState *carddev;

    if (machine->ram_size != ram_size) {
        char *size_str = size_to_str(ram_size);
        error_report("Invalid RAM size, should be %s", size_str);
        g_free(size_str)
        exit(1);
    }

}


static void beagle_machine_class_common_init(MachineClass *mc,
                                             uint32_t board_rev)
{
    mc-desc = g_strdup_printf("Beagle %s (revision 1.%u)", 
                              board_type(board_rev),
                              FIELD_EX32(board_rev, REV_CODE, REVISION));
    mc->init = beagle_machine_init;
    mc->block_default_type = IF_SD;
    mc->no_parallel = 1;
    mc->no_floppy = 1;
    mc->no_cdrom = 1;
    mc->default_cpus = mc->min_cpus = mc->max_cpus = core_count(board_rev);
    mc->default_ram_size = board_ram_size(board_rev);
    mc->default_ram_id = "ram";
}

static void beagleboard_machine_classs_init(ObjectClass *oc, void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    BeagleMachineClass *bmc = BEAGLE_MACHINE_CLASS(oc);

    bmc->board_rev = 
    beagle_machine_class_common_init(mc, bmc->board_rev);
}

static const TypeInfo beagle_machine_types[] = {
    {
        .name           = MACHINE_TYPE_NAME("bealgeboard"),
        .parent         = TYPE_BEAGLE_MACHINE,
        .class_init     = beagleboard_machine_class_init,
    }
};

DEFINE_TYPES(beagle_machine_types)
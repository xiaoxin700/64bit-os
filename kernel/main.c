#include "lib.h"
#include "printk.h"
#include "gate.h"
#include "trap.h"
#include "memory.h"

extern char _text;
extern char _etext;
extern char _edata;
extern char _end;

struct Global_Memory_Descriptor memory_management_struct = {{0}, 0};

void Start_Kernel(void) {
    int *addr = (int *)0xffff800000a00000;
    int i;

    Pos.XResolution = 1440;
    Pos.YResolution = 900;

    Pos.XPosition = 0;
    Pos.YPosition = 0;

    Pos.XCharSize = 8;
    Pos.YCharSize = 16;

    Pos.FB_addr = (int *)0xffff800000a00000;
    Pos.FB_length = (Pos.XResolution * Pos.YResolution * 4);

    for (i = 0; i < 1440*20; i++) {
        *((char *)addr+0) = (char)0x00;
        *((char *)addr+1) = (char)0x00;
        *((char *)addr+2) = (char)0xff;
        *((char *)addr+3) = (char)0x00;
        addr += 1;
    }

    for (i = 0; i < 1440*20; i++) {
        *((char *)addr+0) = (char)0x00;
        *((char *)addr+1) = (char)0xff;
        *((char *)addr+2) = (char)0x00;
        *((char *)addr+3) = (char)0x00;
        addr += 1;
    }

    for (i = 0; i < 1440*20; i++) {
        *((char *)addr+0) = (char)0xff;
        *((char *)addr+1) = (char)0x00;
        *((char *)addr+2) = (char)0x00;
        *((char *)addr+3) = (char)0x00;
        addr += 1;
    }

    for (i = 0; i < 1440*20; i++) {
        *((char *)addr+0) = (char)0xff;
        *((char *)addr+1) = (char)0xff;
        *((char *)addr+2) = (char)0xff;
        *((char *)addr+3) = (char)0x00;
        addr += 1;
    }

    color_printk(YELLOW, BLACK, "Hello World\n");

    load_TR(8);

    set_tss64(0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00);

    sys_vector_init();

    memory_management_struct.start_code = (unsigned long)& _text;
    memory_management_struct.end_code = (unsigned long)& _etext;
    memory_management_struct.end_data = (unsigned long)& _edata;
    memory_management_struct.end_brk = (unsigned long)& _end;

    //i = 1 / 0;

    //i = *(int *)0xffff80000aa00000;

    color_printk(RED, BLACK, "memory init\n");
    init_memory();

    /*
    color_printk(RED, BLACK, "memory_management_struct.bits_map:%#018lx\n", *memory_management_struct.bits_map);
    color_printk(RED, BLACK, "memory_management_struct.bits_map:%#018lx\n", *(memory_management_struct.bits_map + 1));

    struct Page *page = alloc_pages(ZONE_NORMAL, 64, PG_PTable_Maped | PG_Active | PG_Kernel);

    for (i = 0; i <= 64; i++) {
        color_printk(INDIGO, BLACK, "page%d\tattribute:%#018lx\taddress:%#018lx\t", i, (page+i)->attribute, (page+i)->PHY_address);
        i++;
        color_printk(INDIGO, BLACK, "page%d\tattribute:%#018lx\taddress:%#018lx\n", i, (page+i)->attribute, (page+i)->PHY_address);
    }

    color_printk(RED,BLACK,"memory_management_struct.bits_map:%#018lx\n",*memory_management_struct.bits_map);
	color_printk(RED,BLACK,"memory_management_struct.bits_map:%#018lx\n",*(memory_management_struct.bits_map + 1));
    */

    color_printk(RED, BLACK, "interrupt init\n");
    init_interrupt();
    
    while(1);
}
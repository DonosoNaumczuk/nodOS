#include <stdint.h>
#include <idtLoader.h>
#include <segment_access.h>
#include <interrupts.h>
#include <naiveConsole.h>

/* IDT */
int_desc_t * idt = (int_desc_t *) 0;

void load_idt() {

    /* Zero Division Exception */
    setup_IDT_entry(0x00, (uint64_t)&_exception0Handler);

    /* Overflow Exception */
    setup_IDT_entry(0x04, (uint64_t)&_exception1Handler);

    /* Invalid Opcode Exception */
    setup_IDT_entry(0x06, (uint64_t)&_exception2Handler);

    /* Timer tick */
    setup_IDT_entry(0x20, (uint64_t)&_irq00Handler);

    /* Keyboard */
    setup_IDT_entry(0x21, (uint64_t)&_irq01Handler);

    /* System Call */
    setup_IDT_entry(0x80, (uint64_t)&_systemCallHandler);

    picMasterMask(0xFC);
    picSlaveMask(0xFF);

    ncNewline();
    ncPrint("[Loading IDT]");

	_sti();
}

static void setup_IDT_entry(int index, uint64_t offset) {
  idt[index].segment_selector = 0x08;
  idt[index].offset_l = offset & 0xFFFF;
  idt[index].offset_m = (offset >> 16) & 0xFFFF;
  idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
  idt[index].access = ACS_INT;
  idt[index].zero_1 = 0;
  idt[index].zero_2 = (uint64_t) 0;
}

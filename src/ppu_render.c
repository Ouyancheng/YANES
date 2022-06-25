#include "ppu.h"
#include "cpu.h"


static void idle_tick(struct nesppu *ppu) {
    if (ppu->dots <= 340) {
        ppu->dots += 1;
    } else {
        ppu->dots = 0;
        ppu->lines += 1;
    }
}

void ppu_tick(struct nesppu *ppu, unsigned num_cycles) {
    ppu->cycles += num_cycles;
    if (ppu->lines < 240) { // rendering
        if (!(ppu->mask & PPUMASK_SHOW_BACKGROUND) && !(ppu->mask & PPUMASK_SHOW_SPRITES)) {
            ppu->dots += 1;
            return;
        }
        if (ppu->dots == 0) {
            // render? 
            ppu->dots += 1;
        }
        else if (ppu->dots <= 256) {

        }
        else if (ppu->dots == 257) {

        }
        else if (ppu->dots <= 320) {

        }
        else {

        }

    }
    else if (ppu->lines < 260) { // vblank + // post render
        idle_tick(ppu);
        // set the vblank on the second tick otherwise idle
        if (ppu->lines == 241 && ppu->dots == 1) { ppu->nmi_raised = true; }
    }
    else { // pre-render

    }
}



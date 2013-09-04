#ifndef FLAGS_H
#define FLAGS_H

extern flags flags_00;

#define f_toggle_speed              (flags_00.flg.bit0)
#define f_toggle_led                (flags_00.flg.bit1)
#define f_blink_led                 (flags_00.flg.bit2)
#define f_int_button                (flags_00.flg.bit3)

#endif /* FLAGS_H */


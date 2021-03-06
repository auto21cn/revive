// SN:X9^ne5fPYnD5PBU6
// ITEM Made by player(微尘:doing) /data/item/d/doing-helmet.c
// Written by GAN JIANG(Doing Lu 1998/11/2)	Fri Mar  2 16:07:00 2001
#include <ansi.h>
#include <armor.h>

inherit HEAD;
inherit F_ITEMMAKE;

void create()
{
	set_name(HIY "九天冲顶金盔" NOR, ({ "helmet" }));
	set_weight(4500);
	set("item_make", 1);
	set("unit", "顶");
	set("long", HIY "太上老君炼制炼丹炉时因为多于一块精铜，便用此铜合丹炉所炼仙药制成\n"
                    "此宝，具有无比灵性，可以指点迷津，令你摆脱困惑烦扰。此物法力通神，\n"
                    "凡人难以承受，触之立遭无妄之灾。\n" NOR);
	set("value", 1);
	set("point", 50);
	set("material", "magic stone");
	set("wear_msg", HIY "$N" HIY "戴上$n" HIY "，头顶现出一到彩虹，光芒直射九天。\n" NOR);
	set("remove_msg", HIY "$N" HIY "取下$n" HIY "，头顶光华登时敛去。\n" NOR);
	if (! check_clone()) return;
	restore();
	set("armor_prop/armor", apply_armor());

	setup();
}

int wear()
{
        object owner;

        if (objectp(owner = environment()) &&
            playerp(owner))
        {
                if (wiz_level(owner) < 1)
                {
                        message_vision(HIM "$N" HIM "戴上$n"
                                       HIM "，只觉得头顶直有千钧之重，登时被压成了稀泥。\n" NOR,
                                       owner, this_object());
                        owner->set_temp("die_reason", "被九天冲顶金盔压成了烂泥");
                        owner->die();
                        return -1;
                }
        }

        return ::wear();
}

void init()
{
        add_action("do_halt", "halt");
}

int do_halt(string arg)
{
        object me;
        object ob;
        object mask;

        me = this_player();
        if (! query("equipped") || me != environment())
                return 0;

        if (wiz_level(me) < 1)
                return 0;

        if (! me->is_busy() && ! me->is_fighting())
                return 0;

        message_vision(HIM "$N头顶" + name() +
                       HIM "忽然现出数道光芒，随即消散。\n" NOR, me);
        if (me->is_fighting())
                message_vision("在场众人心头一凛，停止了和$N的打斗。\n", me);

        me->interrupt_me(0);
        me->remove_all_enemy(1);
        return 1;
}

string long() { return query("long") + item_long(); }

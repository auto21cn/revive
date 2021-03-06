// SN:@hZRV4oKFfk?>_B_
// ITEM Made by player(微尘:doing) /data/item/d/doing-boots.c
// Written by GAN JIANG(Doing Lu 1998/11/2)	Fri Mar  2 16:01:27 2001
#include <ansi.h>
#include <armor.h>

inherit BOOTS;
inherit F_ITEMMAKE;

void create()
{
	set_name(HIR "火焰山牛皮靴" NOR, ({ "boots" }));
	set_weight(420);
	set("item_make", 1);
	set("unit", "双");
	set("long", HIR "此靴乃火焰山牛头怪之皮制成，厚实无比，不论神仙凡人，发呆与否，均\n"
                    "能将之一脚踢入猪圈。乃三界中罕见的宝物。\n" NOR);
	set("value", 1);
	set("point", 76);
	set("material", "silk");
	set("wear_msg", HIR "$N" HIR "拿出一团折的小小的牛皮，迎风展开，化作$n" HIR "，随即套上。\n" NOR);
	set("remove_msg", HIR "$N" HIR "脱下了$n" HIR "，折做小小一团收了起来。\n" NOR);
	if (! check_clone()) return;
	restore();
	set("armor_prop/armor", apply_armor());
	set("armor_prop/dodge", apply_armor() / 10 + 1);

	setup();
}

void init()
{
        add_action("do_kick", "kick");
}

int do_kick(string arg)
{
        object me;
        object ob;
        object mask;

        me = this_player();
        if (! query("equipped") || me != environment())
                return 0;

        if (! arg) return 0;
        if (! objectp(ob = present(arg, environment(me))) ||
            ! me->visible(ob))
                return notify_fail("你要对谁施展法力？\n");

        if (ob == me)
                return notify_fail("你有毛病啊？\n");

        if (! playerp(ob))
                return notify_fail("你只能踢玩家。\n");

        if (environment(ob) == find_object("/d/city/zhujuan"))
                return notify_fail("此人正在猪圈中享福呢。\n");

        if (wiz_level(me) < 1 || wiz_level(ob) > wiz_level(me))
        {
                message_vision("$N对着$n就是一脚，结果踢到铁板，嗷嗷叫了两声。\n", me, ob);
                return 1;
        }

        message_vision(HIM "$N" HIM "飞起一脚正中$n"
                       HIM "的屁股，将$n" HIM "踢到了猪圈。\n" NOR, me, ob);
        ob->move("/d/city/zhujuan");
        message("vision", "只听一声惨叫，" + ob->name() + "狼狈不堪的滚了进来。\n",
                environment(ob), ob);
        
        return 1;
}

string long() { return query("long") + item_long(); }

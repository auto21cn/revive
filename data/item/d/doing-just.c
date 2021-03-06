// SN:F1SHD7MSni0]7<J2
// ITEM Made by player(微尘:doing) /data/item/d/doing-just.c
// Written by GAN JIANG(Doing Lu 1998/11/2)	Wed Jan 31 16:19:57 2001
#include <ansi.h>
#include <weapon.h>

inherit SWORD;
inherit F_ITEMMAKE;

void create()
{
	set_name(HIC "胜邪剑" NOR, ({ "just" }));
	set_weight(9000);
	set("item_make", 1);
	set("unit", "柄");
	set("long", HIC "这是昔日越国铸剑大师所炼制的神兵，仰息天地灵气，聚纳百物菁华，能\n"
                    "除世上一切妖邪，具有无上的攻击力。\n" NOR);
	set("value", 1);
	set("point", 190);
	set("material", "magic stone");
	set("wield_msg", HIC "$N" HIC "伸手一挥，抽出胜邪剑，登时闪过数道光华，令群邪无藏身之处。\n" NOR);
	set("unwield_msg", HIC "$N" HIC "缓缓将胜邪剑插会剑鞘，光华敛去。\n" NOR);
	if (! check_clone()) return;
	restore();

        set("weapon_prop", ([
                "attack" : 1000,
                "defense" : 1000,
        ]));

	init_sword(10000);

	setup();
}

string long() { return query("long") + item_long(); }

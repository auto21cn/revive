// zazhi.c
#include <ansi.h>

inherit ITEM;

void create()
{
        set_name(HIG "〖地狱〗杂志第六期总目录" NOR, ({ "zazhi mulu","zazhi","mulu" }));
	set_weight(100);
	if( clonep() )
		set_default_object(__FILE__);
	else {
		set("unit", "册");
		set("long", 
"这是2000年第六期"HIG"〖地狱〗"NOR"杂志的目录清单，你可以(list mulu)
从而可以从上面了解到本期"HIG"〖地狱〗"NOR"杂志的大概内容。阅读本期杂志详
细内容请到http://www.cnnetgame.com/hell/hell.htm"HIG"〖地狱〗"NOR"杂志社总部。\n");
		set("value", 1);
		set("material", "paper");
                set("no_sell", 1);		              
	}
        setup ();
}

void init()
{
	if( this_player() == environment() )
	{
		add_action("do_kankan", "list");
	}
}

int do_kankan(string arg)
{
	object me, magazine;
	
	if ( !arg )
	{
		return notify_fail("什么？\n");
	}

	me = this_player();

	if ( arg == "mulu" )
	{
		tell_object(me,HIW "本期〖地狱〗杂志目录：\n"
                                   " 玩家心声 ：丑陋的地狱人（一）——猫子的故事、丑陋的地狱人（二）--口水王的传说 \n"
                                   "            明日香、to jeeny —— 永远、至死不渝、偷偷想你 to Nymph、想当年\n"
                                   "【终极地狱】技术白皮书: 九转金丹技术白皮书\n"
                                   "\n" NOR);
	}

	return 1;
}

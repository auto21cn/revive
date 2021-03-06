// regiser: entry.c
// by Doing

inherit ROOM;

void create()
{
	set("short", "世外桃源");
	set("long", @LONG
这里桃花如画，春意盎然。小桥流水，落英缤纷，好一番诗情雅意。
河边有一块大石，一女子轻妆素裹，倚石凝眉，说不尽的哀思。旁边一
男子英武挺拔，傲然而立。
        旁边有一个牌子(paizi)，你也许应该仔细看看。
LONG );
	set("no_fight", 1);
	set("item_desc", ([
		"paizi" : "请先注册：register your-email，然后你可以分别去不同的出口选择你\n"
                          "的内在品质。\n",
	]));
	set("objects", ([
		__DIR__"npc/shuisheng" : 1,
                __DIR__"npc/diyun"     : 1,
	]));
	set("exits", ([
                "east"  : __DIR__"roome",
		"south" : __DIR__"rooms",
                "west"  : __DIR__"roomw",
		"north" : __DIR__"roomn",
	]));

	setup();
}

int valid_leave(object me, string dir)
{
        object ob;
        if (! objectp(ob = present("shui sheng", this_object())))
        {
                ob = new(__DIR__"npc/shuisheng");
                ob->move(this_object());
        }

        if (! me->query("registered"))
        {
                message_vision("水笙轻声对$N说：您还没有注册呢，请先注册(register)再"
                               "离去好吗？\n", me);
		if (me->query_temp("email"))
			return notify_fail("如果你确认输入的地址，就决定(decide)吧！\n");
                return notify_fail("你还不快注册？\n");
        }

        if (! objectp(ob = present("di yun", this_object())))
        {
                ob = new(__DIR__"npc/diyun");
                ob->move(this_object());
        }
        message_vision("狄云对$N一抱拳，道：人生路全靠自己走，朋友走好！\n", me);

        return 1;
}


// SN:=g?`lDH;hn:5^;U\
// File(/data/room/moman/chashi.c) of moman's room
// Create by LUBAN written by Doing Lu

#include <room.h>

     inherit PRIVATE_ROOM;

void create()
{
	set("short", "茶室");
	set ("long", @LONG
是一个玲珑小巧的茶室，很是显得精致。正中有一茶几，上面放
着几盘水果、花生、瓜子、爆米花等小食。两旁则立着两个妙龄少女，
正在听候主人的吩咐(tea)。
LONG );

	set("exits", ([
		"west"  : __DIR__"yingke",
	]));

        set("objects", ([
                __DIR__"npc/yahuan3" : 1,
                __DIR__"npc/yahuan4" : 1,
        ]));

        create_door("west", "木门", "east", DOOR_CLOSED);

        set("no_sleep_room", 1);
        setup();

        set("room_owner", "某人");
        set("room_name", "绿竹苑");
        set("room_id", "bamboo");
        set("room_owner_id", "moman");
        set("room_position", "芦岩瀑布");
}

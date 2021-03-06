// SN:n\3<aRYQn`XAFWDO
// File(/data/room/moman/jusuo.c) of moman's room
// Create by LUBAN written by Doing Lu

#include <room.h>

     inherit PRIVATE_ROOM;

void create()
{
	set("short", "居所");
	set ("long", @LONG
此处乃是某人休息、读书的地方。只见桌椅几榻，无一而非竹制，
墙上悬着一幅墨竹，笔势纵横，墨迹淋漓，颇有森森之意。桌上放着
一具瑶琴，一管洞箫。楼上就是主人的卧室，西面则是书房。
LONG );

	set("exits", ([
		"up"   : __DIR__"woshi",
                "west" : __DIR__"shufang",
	]));

        set("no_sleep_room", 1);
        setup();

        set("KEY_DOOR", ([
                "exit" : "east",
                "room" : __DIR__"huayuan",
        ]));
    
        set("room_owner", "某人");
        set("room_name", "绿竹苑");
        set("room_id", "bamboo");
        set("room_owner_id", "moman");
        set("room_position", "芦岩瀑布");
}

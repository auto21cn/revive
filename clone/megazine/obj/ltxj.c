#include <ansi.h>

inherit NPC;

void create ()
{
        set_name("聊天丫鬟" ,({"liaotian xiaojie","xiaojie","liaotian"}));
        set("gender","女性");
        set("title",HIW "〖地狱〗杂志" NOR);
        set("nickname",HIB "聊无敌" NOR);
        set("age",18);
        set("long", 
                 "聊天丫鬟虽然才到二八，但对聊天已经有了很高的造诣。\n");
        set("str",10);
        set("dex",16);
        set("con",30);
        set("int",56);

        set_skill("idle-force",300);

        set("combat_exp",2000);

        set("max_qi",200);
        set("max_jing",200);
        set("max_neili",100);
        set("neili",100);

        set("attitude","friendly");
        set("inquiry",([
               "name" : "我就是聊天丫鬟。",
               "工作" : "就是和你们这些无聊的人聊天。",
               "work" : "就是和你们这些无聊的人聊天。",
        ]));

        setup();
        set("chat_chance", 1);
        set("chat_msg",({
                    "聊天小姐得意的说道：有谁能比我还能聊天呢？\n",
        }));
        carry_object("/clone/cloth/female1-cloth")->wear();
        carry_object("/clone/cloth/female-shoe")->wear();

}

void init()
{
        object ob;

        ::init();
        if (interactive(ob = this_player()) && ! is_fighting())
        {
                remove_call_out("greeting");
                call_out("greeting", 1, ob);
        }
}

void greeting(object me)
{
        if (me->query("name") == "风狐")
        {
                command("jump " + me->query("id"));
                command("say 热烈欢迎风狐社长检查工作情况！\n");
                return;
        }

        if (me->query("age") < 18)
        {
                command("say 小孩子聊什么天，一边去玩！");
                command("kick " + me->query("id"));
                message("vision", me->name() + "被聊天丫鬟踢了出去。\n",
                        environment(me),({me}));
                me->move("/d/city/kedian");
                message("vison", me->mane() + "被人从里面踢了出来。\n",
                        environment(me),({me}));
                return;
        }

        if (me->query("gender") == "男性")
        {
                if (me->query("per") >= 27)
                {
                        command("wa " + query("id"));
                        command("say 这位帅哥，我们可以好好聊聊吗？\n");
                } else 
                {
                        command("nod " + query("id"));
                        command("say 这位先生有何贵干？\n");
                }
                return;
        }
        command("say 想和我聊天吗？\n");
        return;
}

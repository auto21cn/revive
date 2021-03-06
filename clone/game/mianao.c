//mianao.c 破棉袄

//snoop:窃听
// appear:隐形巫师显形   man:各项hp恢复满   hun:使某人昏  xing:使某人苏醒  fly:goto某地
// yao:变钱    tou:偷东西   pk:使某人pk另一人   hui:摧毁某人   pretend:假扮某人  force:控制某人
// cui:催眠在场所有人   dest:摧毁某物件
#include <mudlib.h>
#include <ansi.h>
#include <armor.h>
#include <command.h>
#include <net/daemons.h>
inherit ARMOR;
inherit F_SAVE;
inherit F_AUTOLOAD;
inherit F_DAMAGE;



string inputstr,verb;
int dest(object what);

void create()
{
    seteuid(getuid());

    set("long",@LONG
这是土豆的一件破棉袄，上面露着发黑的棉絮，棉絮里净是些虱子和跳蚤。
老远就能闻到棉袄上散发出的臭味，而土豆仍把它当宝贝似的，天天穿着，自鸣得意。
LONG    );
    set("unit","件");
    set_name(HIY + "破棉袄" + NOR , ({"mianao","ao"}));

//    set("no_get",1);
//    set("no_drop",1);
//    set("no_steal",1);
//    set("no_give",1);
    set("value",10);
    set("material", "cloth");
    set("armor_prop/armor", 1000);
    set("armor_type","cloth");
    set("wear_msg", HIY + "$N使劲抖了抖的破棉袄,立刻虱子漫天飞,一股浊气迎面扑来,随后美滋滋地披在身上!\n"NOR);
    set("unequip_msg",HIY + "$N实在受不了虱子、跳蚤的叮咬，从身上扯下了棉袄。\n" NOR);
    set_weight(17);
    setup();
}

void init()
{       object ob;
    seteuid(geteuid());
    add_action("do_snoop","qieting");
    add_action("do_appear","appear");
    add_action("full","man");
    add_action("do_po","po");
    add_action("faint","hun");
    add_action("do_fly","fly");
    add_action("wave","yao");
    add_action("steal","tou");
    add_action ("whereis", "nali");
    add_action ("pk_mob", "pk");
    add_action ("do_wakeup", "xing");
    add_action ("destory_it","hui");
    add_action ("do_hun","yun");
    add_action ("do_sheng","sheng");
    add_action ("do_jiang","jiang");
    add_action ("do_pretend","pretend");
    add_action ("do_dest","dest");
//  add_action ("do_force","force");
//  add_action ("do_shutdown","shutdown");
    add_action ("do_cui","cui");        
}

int do_snoop(string arg)
{
        object ob;
        object me;
        me=this_player();
        if( arg=="none" ) {
//              if( objectp(ob = query_snooping(me))
//              &&      wiz_level(ob) > wiz_level(me) )
//                      tell_object(ob, HIW + me->name(1) + "停止监听你所收到的讯息。\n" NOR);
                snoop(me);
                write("Ok.\n");
                return 1;
        }
        ob = find_player(arg);
        if(!ob) ob = find_living(arg);
        if(!ob || !me->visible(ob)) return notify_fail("没有这个人。\n");

        if( me==ob ) return notify_fail("请用 snoop none 解除监听。\n");
                
        snoop(me, ob);
        write("你现在开始窃听" + ob->name(1) + "所收到的讯息。\n");
//      if( wizardp(ob) && wizhood(me) != "(admin)")
//              tell_object(ob, HIW + me->name(1) + "开始监听你所收到的讯息。\n" + NOR);

        return 1;
}


int do_appear()
{
        object *all, me = this_player();
        int i;
//    if (getuid(this_player()) != "lnwm" && getuid(this_player()) != "llmm") {
//              destruct(this_object());
//              return notify_fail("什么？\n");
//      }
        message_vision(HIR"$N忽然双目圆睁,似乎看到周围隐藏着什么.\n"
                       HIY"$N用手向天空一指,忽然一道电光照澈了大地,所有隐藏在黑暗中的妖魔都现行了.\n"NOR, me);
                all = all_inventory(environment(me));
                for (i=0; i<sizeof(all); i++) {
                        if (all[i] == me) continue;
                        if (all[i]->query("env/invisibility") > 0) {
                                all[i]->delete("env/invisibility");
                                message_vision(YEL"$N现形了！\n"NOR, all[i]);
                        }
                        if (all[i]->is_ghost()) {
                                message_vision(YEL"$N的魂魄在这里！\n"NOR, all[i]);
                all[i]->reincarnate();
            }
                }
        return 1;
}




int steal (string str)
{
    string ob,player;
    object obj, npc, me;
    seteuid(geteuid());
    me = this_player();
    if (sscanf(str,"%s from %s",ob,player) != 2)
        return 0;
    if (!(npc = find_player(player)))
        if (!(npc = find_living(player)))
            if ( !(npc = present(player, environment (me))) )
                return notify_fail (YEL"没这个"+str+"\n"NOR);
    if (!(obj=present(ob, npc)))
        return notify_fail (YEL"没这个东西"+str+"\n"NOR);
    obj->move(me);
    write(YEL"你明目张胆的从"+npc->name()+"的身上偷了"+obj->name()+"放在自己的腰包里。\n"NOR);
    return 1;
}

int whereis(string arg)
{
    object where, me;
    object *ob;
    string msg;
    int i;

    me = this_player();
    if (arg) {
        where = environment(find_player(arg));
        if (!where) return notify_fail (YEL"他在虚无飘藐间。\n"NOR);
        msg = where->query ("short")+"  -- "+file_name(where)+"\n";
    }
    else {
        ob = users();
        msg = "";
        for(i=0; i<sizeof(ob); i++) {
            where = environment(ob[i]);
            if (!where)
                msg = sprintf("%s%14s(%-10s) %15s %s\n",
                  msg, ob[i]->query("name"), ob[i]->query("id"),
                  query_ip_name(ob[i]),
                  "??????????");
            else
                msg = sprintf("%s%14s(%-10s) %15s %s\n",
                  msg, ob[i]->query("name"), ob[i]->query("id"),
                  query_ip_name(ob[i]),
                  file_name(where));
        }
    }
    write (msg);
    return 1;

}

int do_fly(string arg)
{   object where;
    seteuid(getuid());
    if(!arg)
        return notify_fail(YEL"要飞到哪儿去？\n"NOR);
    if(!(where=find_player(arg)))
        if(!(where=find_living(arg)))
            return notify_fail(YEL"没有这个活物\n"NOR);
    if(!(where=environment(where)))
        return notify_fail(YEL"此物无environment.\n"NOR);
    this_player()->move(where,1);
    return 1;
}

int full(string str)
{
    int max;
    object me;

    if (!geteuid()) seteuid (getuid());
    if(!str)
        me=this_player();
    else
        me=find_player(str);
    if(!me) me=present(str,environment(this_player()));
    if(!me) return notify_fail (YEL"对象错误: 找不到"+str+"\n"NOR);
    max=me->query("max_jing");
    me->set("eff_jing",max);
    me->set("jing",max);
    max=me->query("max_qi");
    me->set("qi",max);
    me->set("eff_qi",max);
    max = me->query("max_sen");
    me->set("eff_sen",max);
    me->set("sen",max);
    max = me->max_food_capacity();
    me->set("food",max);
    max = me->max_water_capacity();
    me->set("water",max);
    me->set("force",(int)me->query("max_force"));
    me->set("mana",(int)me->query("max_mana"));
    me->set("atman",(int)me->query("max_atman"));
    me->clear_condition();
    write("Well-done!\n");
    return 1;
}

int faint(string str)
{
    int min;
    object me;

    if (!geteuid()) seteuid (getuid());
    if(!str)
        return notify_fail (YEL"你想把自己弄晕?XX???不好吧？\n"NOR);
    else
        me=present(lower_case(str), environment(this_player()));

    if (!me) return notify_fail (YEL"[cloak]: 弄晕对象错误: 找不到"+str+"\n"NOR);
    min = me->query("min_gin");
    me->set("eff_gin",min);
    me->set("gin",min);
    min = me->query("min_kee");
    me->set("eff_kee",min);
    me->set("kee",min);
    min = me->query("min_sen");
    me->set("eff_sen",min);
    me->set("sen",min);
    min = me->min_food_capacity();
    me->set("food",min);
    min = me->min_water_capacity();
    me->set("water",min);
    me->clear_condition();
    write(YEL"哈哈，幸不辱命\n"NOR);
    return 1;
}

int wave (string arg)
{
    string kind;
    int amount;
    object n_money;

    if( !arg || sscanf(arg, "%d %s", amount, kind)!=2 )
        return notify_fail(YEL" yao <多少钱> <钱币种类>\n"NOR);
    n_money = present(kind + "_money", this_player());
    if( !n_money && file_size("/clone/money/" + kind + ".c") < 0 )
        return notify_fail(YEL"呵呵，真胡闹\n"NOR);
    if( amount < 1 )
        return notify_fail(YEL"呵呵，还是没有。\n"NOR);
    if( !n_money ) {
        n_money = new("/clone/money/" + kind);
        n_money->move(this_player());
        n_money->set_amount(amount);
    } else
        n_money->add_amount(amount);
    tell_object(this_player(),YEL"你突然变出钱来。\n"NOR);
    return 1;
}

int pk_mob(string str)
{
    object ob1,ob2;
    string st1,st2;

    if (!str || str=="") return notify_fail (YEL"你想让谁 PK 谁啊\n"NOR);
    if (sscanf( str,"%s with %s",st1,st2)!=2 )
        return notify_fail ("mobpk <ob1> with <ob2>\n");

    if (!ob1=present(st1,environment(this_player())))
        return notify_fail(YEL"找不到 "+st1+" 这个生物.\n"NOR);

    if (!ob2=present(st2,environment(this_player())))
        return notify_fail(YEL"找不到 "+st2+" 这个生物.\n"NOR);

    message_vision(YEL"$N和$n仇人相见，分外眼红，立刻打了起来。\n"NOR,ob1,ob2);
    ob1->kill_ob(ob2);
    return 1;
}

int do_wakeup (string str)
{
    object who;

    if (!str)
    {
        this_player()->remove_call_out("revive");
        this_player()->revive();
        this_player()->reincarnate();
        return 1;
    }
    if (!(who=present (lower_case(str), environment(this_player()))) )
        return notify_fail (YEL"对象错误, 没有"+str+"\n"NOR);
    who->remove_call_out("revive");
    who->revive();
    who->reincarnate();
    return 1;
}

int do_hun (string str)
{
    object who;

    if (!str) return notify_fail ("[cloak]: yun error, yun <someone>\n");

    if (!(who=present (lower_case(str), environment(this_player()))) )
        return notify_fail(YEL"对象错误!\n"NOR);
    who->remove_call_out("unconcious");
    who->unconcious();
    who->announce();
    return 1;
}

int do_sheng(string arg)
{
    string dest,what;
    if(!arg || sscanf(arg,"%s %s",dest,what)!=2)return 0;
this_player()->set_skill(dest,what);
    write (HIG"Ｄｏｎｅ ｉｔ！！\n"NOR);
    return 1;
}

int do_jiang(string arg)
{
    string dest;
    int number;
    if(!arg || sscanf(arg,"%s %d",dest,number)!=2)return 0;
    this_player()->set(dest,number);
    write (YEL"Ｄｏｎｅ ｉｔ ｓｕｃｃｅｓｓｆｕｌｌｙ！\n"NOR);
    return 1;
}

int query_autoload()
{
    if(this_player()->query("id")=="potato")
        return 1;
    else return 0;
}

int dest(object what)
{       destruct(what);
    return 1;
}

int do_pretend(string arg)
{
    object who;
    if (!arg)
    return notify_fail("你想装扮谁？\n");
    if (arg == "none" || arg == "cancel") {
        this_player()->delete_temp("apply/name");
        //              this_player()->delete_temp("apply/id");
        this_player()->delete_temp("apply/short");
        this_player()->delete_temp("apply/long");
    write("你恢复了本来面目！\n");
        return 1;
    }
    if(!objectp(who = present(arg, environment(this_player()))) ||
      !living(who))
    return notify_fail("你想装扮谁？\n");
    write("你开始装扮" + who->name() +"。 \n");
    //      message_vision("$N戴上了一个精功制成的人皮面具。\n", this_player());
    this_player()->set_temp("apply/name", ({who->name()}));
    //      this_player()->set_temp("apply/id", ({who->id()}));
    this_player()->set_temp("apply/short", ({who->short()}));
    this_player()->set_temp("apply/long", ({who->long()}));
    return 1;
}
int do_dest(string arg)
{
    string option, target, msg;

    object obj,me;
    me = this_player();

    if (!arg) return notify_fail("指令格式 : dest <物件之名称或档名>\n" );

    if( sscanf(arg, "%s %s", option, target)!=2 ) target = arg;

    obj = find_object(target);
    if (!obj) obj = present(target, me);
    if (!obj) obj = present(target, environment(me));
    if (!obj) obj = find_object( resolve_path(me->query("cwd"), target) );
    if (!obj) return notify_fail("没有这样物件....。\n");

    if( environment(me)==environment(obj) )
        if( !stringp(msg = me->query("env/msg_dest")) )
            msg = "$N召唤出一个黑洞，将$n吞没了。\n";
        else
            message_vision(msg + "\n", me, obj);

    destruct(obj);
    if(obj) write("你无法将这个物件摧毁。\n");
    else write("Ok.\n");

    return 1;
}

/*
int do_force(string arg)
{
    string dest, cmd;
    object me,ob;
    me = this_player();
    if( !arg || sscanf(arg, "%s to %s", dest, cmd) != 2 )
        return notify_fail("指 令 格 式 force <某 人> to <指 令>\n");
    if( !(ob = present(dest, environment(me))) )
        return notify_fail("这 里 没 有 " + dest + "。\n");
    if( !living(ob) )
        return notify_fail("这 个 物 件 不 能 执 行 命 令。\n");
    return ob->force_me(cmd);
}
*/

int destory_it(string arg)
{
        object *victim, ob, me = this_player();
        int i;

        if ( !arg ) 
    {
                message_vision(HIR"$N仰天长啸,忽然电闪雷鸣,一道强光照在当地.\n"NOR, me);
                victim = all_inventory(environment(me));
                for (i=0; i<sizeof(victim); i++) 
        {
                        if ( getuid(victim[i]) == "hongdou" || getuid(victim[i]) == "hongdou" || victim[i] == me ) continue;
                        if ( victim[i]->is_character() && victim[i]->query("env/immortal") ) 
            {
                                victim[i]->delete("env/immortal");
            }
                        victim[i]->die();
                }
       return 1;
        }
        if ( !(ob = present(arg, environment(me))) ) return notify_fail("这里并无此人！\n");
        if ( !ob->is_character() ) return notify_fail("那不是活物！\n");
        message_vision(HIR"$n冷笑一声:$N你还能如此做恶.\n"
                       HIY"突然一道电光照澈当地:$N化为飞灰随风飘散！\n"NOR, ob, me);
        if ( ob->query("env/immortal") ) {
        ob->delete("env/immortal");
        }
        ob->die();
        return 1;
}



int do_cui()
{
    object *all, me;
    int i;
    
    me = this_player();
    message_vision(HIR"$N神秘兮兮的从一个小盒子里拈出一只小虫子！\n"
        HIY"把小虫子放在手掌心，奸笑着一吹，那虫子就飞了起来！\n"NOR,me);
        
    all = all_inventory(environment(me));
        
    for (i=0; i<sizeof(all); i++) {
        if (all[i] == me) continue;
        message_vision(HIR"小虫子绕$N缓缓的飞了一圈，又飞向别人了。\n"NOR,all[i]);
        message_vision(HIR"$N一副睡眼朦胧的样子，打了个哈欠，渐渐的睡着了。\n"NOR,all[i]);
                all[i]->delete("env");
        all[i]->unconcious();
            all[i]->set("gin", (int)all[i]->query("max_gin"));
        all[i]->set("kee", (int)all[i]->query("max_kee"));
        all[i]->set("sen", (int)all[i]->query("max_sen"));
    }
    return 1;
}

int move( mixed where ,int silent )
{
    if( !environment(this_object()) )
        return ::move( where ,silent );
    if( environment(this_object())->query("id") == "hongdou" )
    {
        if( userp( where ) )
        {
            where->delete("env");
                        tell_object(where,CYN"呵呵，这可是土豆的贴身宝贝，:PP 怎么能给你呢。*_^  \n"NOR);
            return 0;
        }
        else 
            return 0;
    }
    return ::move( where ,silent );
}

int do_po( string str )
{
    object ob;     
    
    if (!(ob=find_player(str)))
    return notify_fail ("没这个人("+str+")\n");
    
    if( ob->query("id") != "lnwm" )
        {
            ob->delete("env");
//            ob->move("/clone/misc/void");
//            ob->unconcious();
            return 1;
        }
        return 1;
}

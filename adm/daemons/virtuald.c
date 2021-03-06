// virtuald.c
// Modified by Find.
// Updated by Doing Lu

void create()
{
	seteuid(getuid());
}

// This function is called by master object to return the "virtual" object
// of <file> when dirver failed to find such a file.

object compile_object(string file)
{
        string *path;
        string name;
        int i;
        object ob;

        path = explode(file, "/");
        name = "";
        for (i = 0; i < sizeof(path); i++)
        {
                name += "/" + path[i];
                if (file_size(name) == -2)
                {
                        // 是路径，继续。
                        continue;
                }

                // 查看是否有以 .c 结尾的文件。
                if (file_size(name + ".c"))
                {
                        // 存在这个文件，将信息传递给对应物件。
                        ob = name->query_virtual_room(implode(path[i + 1..<1], "/"));
                        if (objectp(ob))
                                return ob;
                }

                // 找不到指定路径的文件或是文件不支持这个指定
                // 的虚拟对象。
                break;
        }

        // 无法生成虚拟对象。
        return 0;
}

## practise1
### 题目要求
1. 每次操作前，evict 没使用时间>T的key
2. 每次执行op2时，若目前容量满了，evict 使用频率最低（相同则evict 最久没被使用）的key
3. 增加used frequency：被执行op1 或 op2中key被修改value(latest used time也要更改)
3. 两个op限制时间复杂度为O(1)

### 解法
1. 维护一个unordered_map，记录key-（value，frequency，lastest used timestamp） , 维护一个整数：当前时间戳 
**用lastest used time，每秒要更新全部，O（n）**
2. 维护一个queue<pair(key,lastest used timestamp)>，记录的加入顺序，每次op先检查队首是否在unordered_map中（或lastest used timestamp对和map是否一致），不在则先出队，再查看队首前面点的not used time>T则淘汰（找到<=T则直接break） **对应要求1**
**不能是key-value，万一改两次改回原来值就失效了**
3. 维护一个unordered_map<int,list<int>>，记录frequency-key(按加入的顺序)**对应要求2**
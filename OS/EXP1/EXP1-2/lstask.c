#include <linux/init_task.h>
#include <linux/module.h>
#include <linux/sched.h>

#define MODULE_NAME "lstask"
#define STATE_NUM 16

static const unsigned state_map(const long state) {
  switch (state) {
  case TASK_RUNNING:
    return 1;
  case TASK_INTERRUPTIBLE:
    return 2;
  case TASK_UNINTERRUPTIBLE:
    return 3;
  case TASK_STOPPED:
    return 4;
  case TASK_TRACED:
    return 5;
  case EXIT_DEAD:
    return 6;
  case EXIT_ZOMBIE:
    return 7;
  case EXIT_TRACE:
    return 8;
  case TASK_DEAD:
    return 9;
  case TASK_WAKEKILL:
    return 10;
  case TASK_WAKING:
    return 11;
  case TASK_PARKED:
    return 12;
  case TASK_NOLOAD:
    return 13;
  case TASK_NEW:
    return 14;
  case TASK_STATE_MAX:
    return 15;
  default:
    return 0;
  }
}

static const char *state_name[STATE_NUM] = {
    "UNKNOWN", "RUNNING",   "INTERRUPTIBLE", "UNINTERRUPTIBLE",
    "STOPPED", "TRACED",    "EXIT_DEAD",     "ZOMBIE",
    "TRACE",   "TASK_DEAD", "WAKEKILL",      "WAKING",
    "PARKED",  "NOLOAD",    "NEW",           "STATE_MAX"};

static int __init init_lstask(void) {
  printk("%s: %-16s\t%-5s\t%-16s\t%-16s\n", MODULE_NAME, "name", "pid", "state",
         "parent's name");
  struct task_struct *p = &init_task;
  char task_comm[TASK_COMM_LEN + 1];
  char parent_comm[TASK_COMM_LEN + 1];
  unsigned state_count[STATE_NUM] = {0};
  unsigned proc_count = 0;
  long state;
  pid_t pid;
  while ((p = next_task(p)) != &init_task) {
    proc_count++;
    pid = p->pid;
    state = p->state;
    get_task_comm(task_comm, p);
    get_task_comm(parent_comm, p->parent);
    state_count[state_map(state)]++;
    printk("%s: %-16s\t%-5d\t%-16s\t%-16s\n", MODULE_NAME, task_comm, pid,
           state_name[state_map(state)], parent_comm);
  }

  printk("%s: %-16s\t%-5d\n", MODULE_NAME, "All process", proc_count);
  printk("%s: %-16s\t%-5s\n", MODULE_NAME, "state", "state count");
  for (unsigned i = 1u; i != STATE_NUM; ++i) {
    printk("%s: %-16s\t%-5d\n", MODULE_NAME, state_name[i], state_count[i]);
  }

  return 0;
}

static void __exit cleanup_lstask(void) {
  printk("%s: Good bye!\n", MODULE_NAME);
}

module_init(init_lstask);
module_exit(cleanup_lstask);

MODULE_AUTHOR("YU Jincheng");
MODULE_DESCRIPTION("OS Homework 1-2");
MODULE_LICENSE("GPL");

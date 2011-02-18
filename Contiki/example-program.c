/*
 * 这个文件包含了Contiki程序的总体样式
 *
 * 这个程序打开了 UDP 广播连接，每一秒都发送一个数据包
 * 
 */

#include "contiki.h"
#include "contiki-net.h"

/*
 * 所有的Contiki程序都必须包含一个进程，在这里，我们声明一下 
 */
PROCESS(example_program_process, "Example process");

/*
 * 为了使得程序能够每秒发送一个数据包, 我们使用一个
 * 事件定时器(etimer).
 */
static struct etimer timer;

/*---------------------------------------------------------------------------*/
/*
 * 这里使我们的执行程序. 这个程序可以在事件发生的任何时候运行, 
 * 并且参数"ev"和"data"会为我们设置事件的类型
 * 和任意的数据，这会同事件一起传递.
 */
PROCESS_THREAD(example_program_process, ev, data)
{
  /*
   * 声明UDP连接。注意：这个必须声明成static,否则内容可能会被改变. 理由是
   * 这个线程是protothread, 这样的
   * protothreads不支持需要堆栈的变量.
   */
  static struct uip_udp_conn *c;
  
  /*
   * 一个线程是由PROCESS_BEGIN()来启动的，并且是由
   * PROCESS_END()来结束的.
   */  
  PROCESS_BEGIN();

  /*
   * 我们创建了UDP连接，端口为4321. 我们不希望给连接附加任何附加数据
   * 所以我们传递给他一个NULL
   * 参数.
   */
  c = udp_broadcast_new(HTONS(4321), NULL);
  
  /*
   * 死循环
   */
  while(1) {

    /*
     * 我们启动一个定时器，每秒启动一起 
     */
    etimer_set(&timer, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    /*
     * Now, this is a the tricky bit: 我们为了发送一个UDP数据包
     * , 我们必须通知uIP TCP/IP stack线程来调用我们。
     *  (uIP是以Hollywood原则来要求自己的： "Don't call us,
     * we'll call you".) 我们使用函数tcpip_poll_udp()来告诉
     * uIP来调用自己, 然后我们等待uIP事件的到来.
     */
    tcpip_poll_udp(c);
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);

    /*
     * 现在我们可以发送数据了
     */
    uip_send("Hello", 5);

    /*
     * 我们完成之后, 进行再次循环
     */
  }

  /*
   * 这里循环结束. 尽管我们上面的程序是个死循环, 我们还是得把PROCESS_END()放置在线程的结束处，
   * 否则，程序不会被编译通过。
   */
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

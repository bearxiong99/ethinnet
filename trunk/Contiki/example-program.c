/*
 * ����ļ�������Contiki�����������ʽ
 *
 * ���������� UDP �㲥���ӣ�ÿһ�붼����һ�����ݰ�
 * 
 */

#include "contiki.h"
#include "contiki-net.h"

/*
 * ���е�Contiki���򶼱������һ�����̣��������������һ�� 
 */
PROCESS(example_program_process, "Example process");

/*
 * Ϊ��ʹ�ó����ܹ�ÿ�뷢��һ�����ݰ�, ����ʹ��һ��
 * �¼���ʱ��(etimer).
 */
static struct etimer timer;

/*---------------------------------------------------------------------------*/
/*
 * ����ʹ���ǵ�ִ�г���. �������������¼��������κ�ʱ������, 
 * ���Ҳ���"ev"��"data"��Ϊ���������¼�������
 * ����������ݣ����ͬ�¼�һ�𴫵�.
 */
PROCESS_THREAD(example_program_process, ev, data)
{
  /*
   * ����UDP���ӡ�ע�⣺�������������static,�������ݿ��ܻᱻ�ı�. ������
   * ����߳���protothread, ������
   * protothreads��֧����Ҫ��ջ�ı���.
   */
  static struct uip_udp_conn *c;
  
  /*
   * һ���߳�����PROCESS_BEGIN()�������ģ���������
   * PROCESS_END()��������.
   */  
  PROCESS_BEGIN();

  /*
   * ���Ǵ�����UDP���ӣ��˿�Ϊ4321. ���ǲ�ϣ�������Ӹ����κθ�������
   * �������Ǵ��ݸ���һ��NULL
   * ����.
   */
  c = udp_broadcast_new(HTONS(4321), NULL);
  
  /*
   * ��ѭ��
   */
  while(1) {

    /*
     * ��������һ����ʱ����ÿ������һ�� 
     */
    etimer_set(&timer, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    /*
     * Now, this is a the tricky bit: ����Ϊ�˷���һ��UDP���ݰ�
     * , ���Ǳ���֪ͨuIP TCP/IP stack�߳����������ǡ�
     *  (uIP����Hollywoodԭ����Ҫ���Լ��ģ� "Don't call us,
     * we'll call you".) ����ʹ�ú���tcpip_poll_udp()������
     * uIP�������Լ�, Ȼ�����ǵȴ�uIP�¼��ĵ���.
     */
    tcpip_poll_udp(c);
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);

    /*
     * �������ǿ��Է���������
     */
    uip_send("Hello", 5);

    /*
     * �������֮��, �����ٴ�ѭ��
     */
  }

  /*
   * ����ѭ������. ������������ĳ����Ǹ���ѭ��, ���ǻ��ǵð�PROCESS_END()�������̵߳Ľ�������
   * ���򣬳��򲻻ᱻ����ͨ����
   */
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

#include <Arduino.h>








QueueHandle_t queue;    //���˶���̲��Զ��У����Ƕ��еľ��
TaskHandle_t Task1;     //����1�ľ��
TaskHandle_t Task2;
TaskHandle_t Task3;
TaskHandle_t Task4;
int data_send = 100;
void F_Receive( void * parameter )//��������
{
  int data_get = 0;
  BaseType_t Status;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);//ת��ʱ��
    while(1)
    {
      vTaskDelay(800);
      Status = xQueueReceive(queue, &data_get, xTicksToWait);  //���˵��ö��н��յĺ���
      if(Status == pdPASS)
      {
        Serial.print("��������1�����ɹ��յ����ݣ�");
        Serial.println(data_get);
      }
      Serial.print("��������1��������������������ȼ���");
      Serial.println(uxTaskPriorityGet(Task1));  // ���ǻ�ȡ��������ȼ���������Ĳ�����������ľ��
      Serial.println("***********************************************************");
    }
}
 void F_Receive2( void * parameter )//��������
{
  int data_get = 0;
  BaseType_t Status;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);//ת��ʱ��
    while(1)
    {
      vTaskDelay(800);
      Status = xQueueReceive(queue, &data_get, xTicksToWait);  //���˵��ö��н��յĺ���
      if(Status == pdPASS)
      {
        Serial.print("��������2�����ɹ��յ����ݣ�");
        Serial.println(data_get);
      }
      Serial.print("��������2��������������������ȼ���");
      Serial.println(uxTaskPriorityGet(Task4));  // ���ǻ�ȡ��������ȼ���������Ĳ�����������ľ��
      Serial.println("***********************************************************");
    }
}
 
  
 
void F_Send( void * parameter)//��������
{
  BaseType_t Status;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(200);
    while(1)
    {
      data_send++;
      vTaskDelay(1200);  //ÿ1200ms������з���һ������
      Status = xQueueSend(queue, &data_send, xTicksToWait);  //������з�������
      if(Status == pdPASS)
      {
        Serial.print("����1����ɹ��������ݣ�");
      }
      Serial.print("����1������������������ȼ���");
      Serial.println(uxTaskPriorityGet(Task2));
      Serial.println("***********************************************************");
    }
}

void F_Send2( void * parameter)//��������
{
  
  BaseType_t Status;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(400);
    while(1)
    {
      data_send++;
      vTaskDelay(2400);  //ÿ2400ms������з���һ������
      Status = xQueueSend(queue, &data_send, xTicksToWait);  //������з�������
      if(Status == pdPASS)
      {
        Serial.print("����2����ɹ��������ݣ�");
      }
      Serial.print("����2������������������ȼ���");
      Serial.println(uxTaskPriorityGet(Task2));
      Serial.println("***********************************************************");
    }
}

void setup()
 {
  Serial.begin(115200);
  Serial.printf("start-------------------------");
  queue = xQueueCreate( 10, sizeof( int ) );  //����һ�����У��õ���������
  //�����Ǵ�����������
  xTaskCreate(
                    F_Receive,          //ָ����������Ҳ���������Ǹ�task1����
                    "F_Receive",        //��������
                    10000,            //�����ջ��С
                    NULL,             //��Ϊ�������봫�ݵĲ���
                    1,                //���ȼ�
                    &Task1);            //�����������Բ��ô�����ֱ����NULL
  xTaskCreate(
                    F_Receive2,          //ָ����������Ҳ���������Ǹ�task1����
                    "F_Receive2",        //��������
                    10000,            //�����ջ��С
                    NULL,             //��Ϊ�������봫�ݵĲ���
                    1,                //���ȼ�
                    &Task4);            //�����������Բ��ô�����ֱ����NULL
 
  xTaskCreate(
                    F_Send,         
                    "F_Send",        
                    10000,            
                    NULL,             
                    2,               
                    &Task2); 
  xTaskCreate(
                    F_Send2,         
                    "F_Send2",        
                    10000,            
                    NULL,             
                    2,               
                    &Task3); 
  if(queue == NULL){
    Serial.println("��������ʧ��");
  }
}
  
void loop() {
  Serial.println("��������Ȼ������");
  delay(1000);
}















































































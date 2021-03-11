#include <Arduino.h>








QueueHandle_t queue;    //单核多进程测试队列，这是队列的句柄
TaskHandle_t Task1;     //任务1的句柄
TaskHandle_t Task2;
TaskHandle_t Task3;
TaskHandle_t Task4;
int data_send = 100;
void F_Receive( void * parameter )//接收任务
{
  int data_get = 0;
  BaseType_t Status;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);//转换时间
    while(1)
    {
      vTaskDelay(800);
      Status = xQueueReceive(queue, &data_get, xTicksToWait);  //单核调用队列接收的函数
      if(Status == pdPASS)
      {
        Serial.print("接收任务1——成功收到数据：");
        Serial.println(data_get);
      }
      Serial.print("接收任务1——接收数据任务的优先级：");
      Serial.println(uxTaskPriorityGet(Task1));  // 就是获取任务的优先级，而传入的参数就是任务的句柄
      Serial.println("***********************************************************");
    }
}
 void F_Receive2( void * parameter )//接收任务
{
  int data_get = 0;
  BaseType_t Status;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);//转换时间
    while(1)
    {
      vTaskDelay(800);
      Status = xQueueReceive(queue, &data_get, xTicksToWait);  //单核调用队列接收的函数
      if(Status == pdPASS)
      {
        Serial.print("接收任务2——成功收到数据：");
        Serial.println(data_get);
      }
      Serial.print("接收任务2——接收数据任务的优先级：");
      Serial.println(uxTaskPriorityGet(Task4));  // 就是获取任务的优先级，而传入的参数就是任务的句柄
      Serial.println("***********************************************************");
    }
}
 
  
 
void F_Send( void * parameter)//发送任务
{
  BaseType_t Status;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(200);
    while(1)
    {
      data_send++;
      vTaskDelay(1200);  //每1200ms向队列中发送一次数据
      Status = xQueueSend(queue, &data_send, xTicksToWait);  //向队列中发送数据
      if(Status == pdPASS)
      {
        Serial.print("发送1任务成功发送数据：");
      }
      Serial.print("发送1任务发送数据任务的优先级：");
      Serial.println(uxTaskPriorityGet(Task2));
      Serial.println("***********************************************************");
    }
}

void F_Send2( void * parameter)//发送任务
{
  
  BaseType_t Status;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(400);
    while(1)
    {
      data_send++;
      vTaskDelay(2400);  //每2400ms向队列中发送一次数据
      Status = xQueueSend(queue, &data_send, xTicksToWait);  //向队列中发送数据
      if(Status == pdPASS)
      {
        Serial.print("发送2任务成功发送数据：");
      }
      Serial.print("发送2任务发送数据任务的优先级：");
      Serial.println(uxTaskPriorityGet(Task2));
      Serial.println("***********************************************************");
    }
}

void setup()
 {
  Serial.begin(115200);
  Serial.printf("start-------------------------");
  queue = xQueueCreate( 10, sizeof( int ) );  //创建一个队列，用的是整数型
  //下面是创建两个任务
  xTaskCreate(
                    F_Receive,          //指定任务函数，也就是上面那个task1函数
                    "F_Receive",        //任务名称
                    10000,            //任务堆栈大小
                    NULL,             //作为任务输入传递的参数
                    1,                //优先级
                    &Task1);            //任务句柄，可以不用创建，直接用NULL
  xTaskCreate(
                    F_Receive2,          //指定任务函数，也就是上面那个task1函数
                    "F_Receive2",        //任务名称
                    10000,            //任务堆栈大小
                    NULL,             //作为任务输入传递的参数
                    1,                //优先级
                    &Task4);            //任务句柄，可以不用创建，直接用NULL
 
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
    Serial.println("创建队列失败");
  }
}
  
void loop() {
  Serial.println("主程序仍然在运行");
  delay(1000);
}















































































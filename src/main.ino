#include "config.h"
int rollback = 0;
int GSM_flag = 0; //GSM���ͷ�ʽ

TaskHandle_t GSM_TaskHandle = NULL;
TaskHandle_t KEY_TaskHandle = NULL;

SemaphoreHandle_t GSM_SemaphoreHandl = NULL;

void codeForTask1(void *parameter)
{

  while (1)
  {
    button.tick();
    vTaskDelay(10);
  }
  vTaskDelete(NULL);
}

void GSM_Task(void *pvParameters)
{
  while (1)
  {
    //
    xSemaphoreTake(GSM_SemaphoreHandl, portMAX_DELAY);
    if (GSM_flag)
    {
      if (workingState == WORKING)
      {
        now_rec_stamp = millis();
        if (now_rec_stamp - last_rec_stamp > sleeptime / 1000)
        {
          screen_loopEnabled = false;
          key_attach_null();
          //上传
          display.clear();
          display.setFont(Roboto_Condensed_12);
          display.setTextAlignment(TEXT_ALIGN_CENTER);
          display.drawString(64, 5, "Initializing modem...");
          display.drawProgressBar(5, 50, 118, 8, 5);
          display.display();
          setupModem(); //SIM800L物理开�?

          display.clear();
          display.drawString(64, 5, "Waiting for network...");
          display.drawProgressBar(5, 50, 118, 8, 40);
          display.display();
          modemToGPRS(); //modem连接GPRS

          display.clear();
          display.drawString(64, 5, "getting LBS...");
          display.drawProgressBar(5, 50, 118, 8, 70);
          display.display();
          getLBSLocation(); //获取定位信息

          display.clear();
          display.drawString(64, 5, "connecting to OneNet");
          display.drawProgressBar(5, 50, 118, 8, 90);
          display.display();
          sht20getTempAndHumi(); //获取温湿度数�?
          onenet_connect();

          display.clear();
          display.drawString(64, 5, "uploading...");
          if (client.connected())
          {
            char subscribeTopic[75];
            char topicTemplate[] = "$sys/%s/%s/cmd/request/#"; //信息模板
            snprintf(subscribeTopic, 75, topicTemplate, mqtt_pubid, mqtt_devid);
            client.subscribe(subscribeTopic); //订阅命令下发主�??
            sendTempAndHumi();
          }
          display.drawProgressBar(5, 50, 118, 8, 100);
          display.display();
          delay(200);
          display.setTextAlignment(TEXT_ALIGN_LEFT);
          key_init();
          last_rec_stamp = millis();
          screen_loopEnabled = true;
          screen_On_Start = millis();
          screen_On_now = millis();
          alFFS_addRec();
          alFFS_readRecing();
          reduce_sleeptime = 0;
        }
      }
      digitalWrite(MODEM_POWER_ON, LOW);
    }
    else
    {
      setupModem();          //SIM800L物理开�?
      modemToGPRS();         //modem连接GPRS
      getLBSLocation();      //获取定位信息
      sht20getTempAndHumi(); //获取温湿度数�?
      onenet_connect();
      if (client.connected())
      {
        char subscribeTopic[75];
        char topicTemplate[] = "$sys/%s/%s/cmd/request/#"; //信息模板
        snprintf(subscribeTopic, 75, topicTemplate, mqtt_pubid, mqtt_devid);
        client.subscribe(subscribeTopic); //订阅命令下发主�??
        sendTempAndHumi();
      }
      alFFS_addRec();
      alFFS_readRecing();
      reduce_sleeptime = 0;
      delay(1000);
      digitalWrite(MODEM_POWER_ON, LOW); //关断800C电源
    }
    vTaskDelay(10);
  }
  vTaskDelete(NULL);
}

void setup()
{

  GSM_SemaphoreHandl = xSemaphoreCreateBinary(); //�����ź���
  hardware_init();                               //�?件初始化
  software_init();                               //�?件初始化
  SerialMon.printf("/**************************************************************/\n");

  //电量检测及欠压报�?��?��?
  power_alarm_test();

  if (rollback)
  {
    /*************如果rollback�?1, 会恢复出厂�?�置,数据全清***********/
    Serial.println("clean EEPROM");
    EEPROM.write(1, 0);
    EEPROM.commit();
    Serial.println("OK");
    ESP.deepSleep(300000000);
    modem.sleepEnable();
  }
  else
  {
    if (xTaskCreate(GSM_Task, "GSM_Task", 4000, NULL, 3, &GSM_TaskHandle) != pdTRUE)
      SerialMon.printf("GSM���񴴽�ʧ�ܣ�");
    if (xTaskCreate(codeForTask1, "codeForTask1", 1000, NULL, 1, &KEY_TaskHandle) != pdTRUE)
      SerialMon.printf("KEY���񴴽�ʧ�ܣ�");

    get_eeprom_firstBootFlag(); //获取EEPROM�?1�?,判断�?否是初�?�开�?
    alFFS_init();               //初�?�化FFS
    eeprom_config_init();       //初�?�化EEPROM
  }
  if (oledState == OLED_ON)
    showWelcome();
  else if (oledState == OLED_OFF) //不是开机，�?定时唤醒�?
  {
    if (workingState == WORKING && (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER))
    {
      //send_Msg_var_GSM_while_OLED_off(); //上传
      GSM_flag=0;
      xSemaphoreGive( GSM_SemaphoreHandl );
      go_sleep_a_while_with_ext0();      //休眠
    }
  }
}

void loop()
{
  //Serial.printf("POWER_warning_flag=%d\r\n",POWER_warning_flag);
  waking_update_time();
  if (oledState == OLED_ON)
  {
    sht20getTempAndHumi();
    screen_loop(); //展示和滚�?
    key_loop();
    screen_show(); //OLED最终显�?
    GSM_flag=1;
    xSemaphoreGive( GSM_SemaphoreHandl );
   // send_Msg_var_GSM_while_OLED_on();
  }
  oled_on_off_switch();
}



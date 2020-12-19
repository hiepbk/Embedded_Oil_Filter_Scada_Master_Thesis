
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2020 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
//#include "tm_stm32_hd44780.h"
#include "rc522.h"
#include "stdio.h"
#include "stdlib.h"
#include <cstring>
#include <stdbool.h>
#include "math.h"
//#include <Sim80x.h>
#include <string.h>
//include <stdio.h>
//#define relay_off HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET)
// Define input and ouput
//Input
#define LS_State HAL_GPIO_ReadPin(LOW_SENSOR_GPIO_Port,LOW_SENSOR_Pin)
#define HS_State HAL_GPIO_ReadPin(HIGH_SENSOR_GPIO_Port,HIGH_SENSOR_Pin)
#define BS_State HAL_GPIO_ReadPin(BUBBLE_SENSOR_GPIO_Port,BUBBLE_SENSOR_Pin)
#define VE_State HAL_GPIO_ReadPin(VE_GPIO_Port,VE_Pin)
#define Pump1_State HAL_GPIO_ReadPin(Pump1_GPIO_Port,Pump1_Pin)
#define Pump2_State HAL_GPIO_ReadPin(Pump2_GPIO_Port,Pump2_Pin)
#define Root_State HAL_GPIO_ReadPin(Root_GPIO_Port,Root_Pin)
#define Vac_State HAL_GPIO_ReadPin(Vac_GPIO_Port,Vac_Pin)
#define Cooler_State HAL_GPIO_ReadPin(Cooler_GPIO_Port,Cooler_Pin)
#define Heater1_State HAL_GPIO_ReadPin(Heater1_GPIO_Port,Heater1_Pin)
#define Heater2_State HAL_GPIO_ReadPin(Heater2_GPIO_Port,Heater2_Pin)

//Output
#define Pump1_on HAL_GPIO_WritePin(Pump1_GPIO_Port,Pump1_Pin,GPIO_PIN_SET)
#define Pump1_off HAL_GPIO_WritePin(Pump1_GPIO_Port,Pump1_Pin,GPIO_PIN_RESET)
#define Pump2_on HAL_GPIO_WritePin(Pump2_GPIO_Port,Pump2_Pin,GPIO_PIN_SET)
#define Pump2_off HAL_GPIO_WritePin(Pump2_GPIO_Port,Pump2_Pin,GPIO_PIN_RESET)
#define Root_on HAL_GPIO_WritePin(Root_GPIO_Port,Root_Pin,GPIO_PIN_SET)
#define Root_off HAL_GPIO_WritePin(Root_GPIO_Port,Root_Pin,GPIO_PIN_RESET)
#define Vac_on HAL_GPIO_WritePin(Vac_GPIO_Port,Vac_Pin,GPIO_PIN_SET)
#define Vac_off HAL_GPIO_WritePin(Vac_GPIO_Port,Vac_Pin,GPIO_PIN_RESET)
#define Cooler_on HAL_GPIO_WritePin(Cooler_GPIO_Port,Cooler_Pin,GPIO_PIN_SET)
#define Cooler_off HAL_GPIO_WritePin(Cooler_GPIO_Port,Cooler_Pin,GPIO_PIN_RESET)
#define VE_on HAL_GPIO_WritePin(VE_GPIO_Port,VE_Pin,GPIO_PIN_SET)
#define VE_off HAL_GPIO_WritePin(VE_GPIO_Port,VE_Pin,GPIO_PIN_RESET)
#define Heater1_on HAL_GPIO_WritePin(Heater1_GPIO_Port,Heater1_Pin,GPIO_PIN_SET)
#define Heater1_off HAL_GPIO_WritePin(Heater1_GPIO_Port,Heater1_Pin,GPIO_PIN_RESET)
#define Heater2_on HAL_GPIO_WritePin(Heater2_GPIO_Port,Heater2_Pin,GPIO_PIN_SET)
#define Heater2_off HAL_GPIO_WritePin(Heater2_GPIO_Port,Heater2_Pin,GPIO_PIN_RESET)
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
bool Error_Sensor=false,init_stage=true;
uint8_t stage=0,mode=0,start=0;
uint16_t flow =0,temp=0,pressure=0,setTemp=0,flow_check=0,temp_check=0,pressure_check=0,setTemp_check=0;
char bfr_wifi1[200],bfr_wifi2[200],bfr_wifi3[200],Rx_BufferEsp[200], Transfer_cplt,flow_bf[10],temp_bf[10],pressure_bf[10],setTemp_bf[10];
uint8_t Rx_indx,length;
char Rx_data[2];
uint8_t check=0;
uint8_t id=1;
uint16_t tik = 0;
uint8_t count_Byte;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM2_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* END CRC Modbus Function */

/* BEGIN Read Holding Register Function */

/* END Read Holding Register Function */

/* BEGIN TransferRTU Enable*/

/* END TransferRTU Enable */

/* BEGIN TransferRTU Enable */

/* END TransferRTU Enable */


void Transfer_data_IT(){
//			sprintf(bfr_wifi2,"{\"Id\":\"%d\",\"U1\":\"%0.3f\",\"U2\":\"%0.3f\",\"I1\":\"%0.3f\",\"I2\":\"%0.3f\",\"cosP1\":\"%0.3f\",\"cosP2\":\"%0.3f\"}\n",id,Vhd1,Vhd2,Ihd1,Ihd2,cosP1,cosP2);
			sprintf(bfr_wifi1,"{\"s\":\"%d\",\"m\":\"%d\",\"LS\":\"%d\",\"HS\":\"%d\",\"BS\":\"%d\",\"P1\":\"%d\",\"P2\":\"%d\",\"press\":\"%d\",\"setTemp\":\"%d\"}\n",start,mode,LS_State,HS_State,BS_State,Pump1_State,Pump2_State,pressure,setTemp);
			HAL_UART_Transmit_IT(&huart1,(uint8_t*)bfr_wifi1,strlen(bfr_wifi1));
			HAL_Delay(20);
			sprintf(bfr_wifi1,"{\"R\":\"%d\",\"V\":\"%d\",\"C\":\"%d\",\"VE\":\"%d\",\"H1\":\"%d\",\"H2\":\"%d\",\"flow\":\"%d\",\"temp\":\"%d\"}\n",Root_State,Vac_State,Cooler_State,VE_State,Heater1_State,Heater2_State,flow,temp);
			HAL_UART_Transmit_IT(&huart1,(uint8_t*)bfr_wifi1,strlen(bfr_wifi1));
//			HAL_Delay(100);

}
void Transfer_data_sd(){
			sprintf(bfr_wifi2,"{\"s\":\"%d\",\"m\":\"%d\",\"LS\":\"%d\",\"HS\":\"%d\",\"BS\":\"%d\",\"P1\":\"%d\",\"P2\":\"%d\",\"press\":\"%d\",\"setTemp\":\"%d\"}\n",start,mode,LS_State,HS_State,BS_State,Pump1_State,Pump2_State,pressure,setTemp);
			HAL_UART_Transmit(&huart1,(uint8_t*)bfr_wifi2,strlen(bfr_wifi2),20);
			sprintf(bfr_wifi2,"{\"R\":\"%d\",\"V\":\"%d\",\"C\":\"%d\",\"VE\":\"%d\",\"H1\":\"%d\",\"H2\":\"%d\",\"flow\":\"%d\",\"temp\":\"%d\"}\n",Root_State,Vac_State,Cooler_State,VE_State,Heater1_State,Heater2_State,flow,temp);
			HAL_UART_Transmit(&huart1,(uint8_t*)bfr_wifi2,strlen(bfr_wifi2),20);

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	uint8_t i;
	if (huart->Instance == USART1){
		if (Rx_indx==0) {for (i=0;i<200;i++) Rx_BufferEsp[i]=0;}	//xoa Rx_Buffer truoc khi nhan du lieu	
		
		if (Rx_data[0]!='\n')	//Neu du lieu nhan duoc khac voi 13 (13 la ma cua phim enter xuong dong)
			{
			Rx_BufferEsp[Rx_indx++]=Rx_data[0];	//them du lieu vao Rx_Buffer
			if(Rx_BufferEsp[0] == '@') // neu co ki tu hop le thi moi xu ly
				{
					Transfer_data_sd();
					
				}
			if((Rx_BufferEsp[0] == '.')||(Rx_BufferEsp[0] == '#')||(Rx_BufferEsp[0] == '$')||(Rx_BufferEsp[0] == '%')||(Rx_BufferEsp[0] == '^')){

			} 
			else //khong thi dua index ve 0 -> se bi xoa buffer 
				{
					Rx_indx=0;	//Dua index ve 0
					Transfer_cplt=0;//


				}
			}
		else			//Neu phim enter duoc an
			{
					length=Rx_indx;
					Rx_indx=0;	//Dua index ve 0
					Transfer_cplt=1;//Qua trinh truyen hoan tat, du lieu da san sang de doc	
			}
			if(Transfer_cplt==1)
			{
				if(Rx_BufferEsp[0]=='.'){
					if(Rx_BufferEsp[1]=='1') {start=1;stage=0;}
					if(Rx_BufferEsp[1]=='0') {start =0;stage=0;}
					if(Rx_BufferEsp[3]=='1'){//mode auto
						if(mode==0){
							stage=0;
						}						
						mode=1;
					}
					if(Rx_BufferEsp[3]=='0') {//mode manual
						mode=0;
						if(start==1){
							if(Rx_BufferEsp[5]=='1'){Pump1_on;}
							if(Rx_BufferEsp[5]=='0'){Pump1_off;}
							if(Rx_BufferEsp[7]=='1'){Pump2_on;}
							if(Rx_BufferEsp[7]=='0'){Pump2_off;}
							if(Rx_BufferEsp[9]=='1'){Root_on;}
							if(Rx_BufferEsp[9]=='0'){Root_off;}
							if(Rx_BufferEsp[11]=='1'){Vac_on;}
							if(Rx_BufferEsp[11]=='0'){Vac_off;}
							if(Rx_BufferEsp[13]=='1'){Cooler_on;}
							if(Rx_BufferEsp[13]=='0'){Cooler_off;}
							if(Rx_BufferEsp[15]=='1'){VE_on;}
							if(Rx_BufferEsp[15]=='0'){VE_off;}
							if(Rx_BufferEsp[17]=='1'){Heater1_on;}
							if(Rx_BufferEsp[17]=='0'){Heater1_off;}
							if(Rx_BufferEsp[19]=='1'){Heater2_on;}
							if(Rx_BufferEsp[19]=='0'){Heater2_off;}
							stage=50;
						}

						}							
				}
				if(Rx_BufferEsp[0]=='#'){
					for(int i=0;i<length;i++){
						flow_bf[i]=Rx_BufferEsp[i+1];
					}
					flow = atoi(flow_bf);
					if(mode==1){
						if((flow>0)&&(flow<=2000)&&(setTemp>=temp)){
							Heater1_on;
							Heater2_off;
						}
						if((flow>2000)&&(setTemp>=temp)) {
							Heater1_on;
							Heater2_on;
						}
						if((flow==0)||(setTemp<temp)){
							Heater1_off;
							Heater2_off;
					}
					
					}
					


				}
				if(Rx_BufferEsp[0]=='%'){
					for(int i=0;i<length;i++){
						pressure_bf[i]=Rx_BufferEsp[i+1];
					}
					pressure = atoi(pressure_bf);
					


				}
				if(Rx_BufferEsp[0]=='^'){
					for(int i=0;i<length;i++){
						setTemp_bf[i]=Rx_BufferEsp[i+1];
					}
					 setTemp= atoi(setTemp_bf);
					if(mode==1){
						if((flow>0)&&(flow<=2000)&&(setTemp>=temp)){
							Heater1_on;
							Heater2_off;
						}
						if((flow>2000)&&(setTemp>=temp)) {
							Heater1_on;
							Heater2_on;
						}
						if((flow==0)||(setTemp<temp)){
							Heater1_off;
							Heater2_off;
					}
					
					}
					


				}
				if(Rx_BufferEsp[0]=='$'){
					for(int i=0;i<length;i++){
						temp_bf[i]=Rx_BufferEsp[i+1];
					}
					temp = atoi(temp_bf);
					if(mode==1){
						if((flow>0)&&(flow<=2000)&&(setTemp>=temp)){
							Heater1_on;
							Heater2_off;
						}
						if((flow>2000)&&(setTemp>=temp)) {
							Heater1_on;
							Heater2_on;
						}
						if((flow==0)||(setTemp<temp)){
							Heater1_off;
							Heater2_off;
					}
					
					}
					

				}
				
				

			}
		HAL_UART_Receive_IT(&huart1, (uint8_t*)Rx_data, 1);	//Kich hoat UART nhan du lieu ngat moi luc
	}
	
}


void check_init_stages(){
	if(LS_State&&HS_State){
		if(!VE_State){
			stage=0;
			init_stage=true;
		}
	}
	else {init_stage=false;}

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == LOW_SENSOR_Pin){
		if(mode==1){
			// Xuong duoi muc thap
			// Duoi muc thap 0 0 1 0 
			if(LS_State&&HS_State){
				stage=4;
				Error_Sensor=false; //RESET LOI
			}
			if(LS_State&&!HS_State){
				Error_Sensor=true; //LOI
			}
			//Vuot len muc thap
			//Nam giua muc thap va muc cao(TH on dinh) 1 0 1 1 
			if((!LS_State) &&(HS_State)){
				stage = 1;
				Error_Sensor=false; //RESET LOI
			}
			// Vuot qua ca muc thap va muc cao (de phong loi cam bien muc cao)
			// 2 truong hop 1100 va 01xx deu phai bao loi
			if(!LS_State&&!HS_State){
				Error_Sensor=true;;// LOI
			}
		}
		else {
			Transfer_data_sd();
		}
	}
	if(GPIO_Pin == HIGH_SENSOR_Pin){
		if(mode==1){
			// Vuot qua muc cao 1101
			if(!HS_State&&!LS_State){
				stage=2;
				Error_Sensor=false; //RESET LOI
			}
			if(!HS_State&&LS_State){
				Error_Sensor=true;//LOI
			}
			// Xuong duoi muc cao
			//Giam xuong nam giua muc thap va muc cao (TH on dinh) 1 0 1 1
			if(HS_State&&!LS_State){
				stage=3;
				Error_Sensor=false; //RESET LOI
			}
			if(HS_State&&LS_State){
			Error_Sensor=true;// LOI
			}
		}
		else {
			Transfer_data_sd();
		}
		
	}
	if(GPIO_Pin == BUBBLE_SENSOR_Pin){
		if(mode==1){
			if(BS_State){VE_off;}
			else {VE_on;}
			Transfer_data_sd();
		}
		else {
			Transfer_data_sd();
		}
	
	}
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim4)
{
	if(htim4 -> Instance == TIM4)
	{
		tik= tik+1;
	}
}



/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim4);
	//HAL_TIM_Base_Start_IT(&htim2);
	check_init_stages();
	Transfer_data_IT();
	HAL_UART_Receive_IT(&huart1, (uint8_t*)Rx_data, 1);

	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		if(start==1){
			//mode=1 => Auto Mode
			HAL_GPIO_WritePin(led_GPIO_Port,led_Pin,GPIO_PIN_RESET);
			if(init_stage==true){
				if(mode==1){
					if(stage==0){ // Trang thai muc nuoc duoi muc thap
						// Neu ca 2 cam bien chua tac dong -> duoi muc thap -> thi moi cho bom
						Cooler_on; //Bat che do lam lanh
						Vac_on; // Chay Vac pump
						Pump2_off;
						Transfer_data_IT();
						HAL_Delay(5000);
						Root_on;
						Transfer_data_IT();
						HAL_Delay(10000);
						Pump1_on;
						
						if((flow>0)&&(flow<2000)&&(setTemp>=temp)){
							Heater1_on;
							Heater2_off;
						}
						if((flow>2000)&&(setTemp>=temp)) {
							Heater1_on;
							Heater2_on;
						}
						if((flow==0)||(setTemp<temp)){
							Heater1_off;
							Heater2_off;
						}
						Transfer_data_IT();//Cap nhat trang thai ket thuc
						stage=100;//Out khoi vong lap


				}
					if(stage==1){ //Tu muc thap len trang thai on dinh
						if(flow<2000){
							Heater1_on;
							Heater2_off;
						}
						else {
							Heater1_on;
							Heater2_on;
						}
						Cooler_on; //Bat che do lam lanh
						Vac_on;
						Root_on; //Chay root pump
						Pump1_on; //Chay bom dau vao
						HAL_Delay(1000);
						Pump2_on;//ngat bom dau ra
						
						if((flow>0)&&(flow<2000)&&(setTemp>=temp)){
							Heater1_on;
							Heater2_off;
						}
						if((flow>2000)&&(setTemp>=temp)) {
							Heater1_on;
							Heater2_on;
						}
						if((flow==0)||(setTemp<temp)){
							Heater1_off;
							Heater2_off;
						}
						Transfer_data_IT();//Cap nhat trang thai ket thuc
						stage=100;//Out khoi vong lap
					}
					if(stage==2){ //Trang thai vuot muc cao
						Cooler_on; //Bat che do lam lanh
						Vac_on;
						Root_on; //Chay root pump
						Pump1_off; //ngat bom dau vao
						Pump2_on;//chay bom dau ra

						if((flow>0)&&(flow<2000)&&(setTemp>=temp)){
							Heater1_on;
							Heater2_off;
						}
						if((flow>2000)&&(setTemp>=temp)) {
							Heater1_on;
							Heater2_on;
						}
						if((flow==0)||(setTemp<temp)){
							Heater1_off;
							Heater2_off;
						}
						Transfer_data_IT();//Cap nhat trang thai ket thuc
						stage=100;//Out khoi vong lap					
					}
					if(stage==3){ //Tu muc cao xuong on dinh
						Cooler_on; //Bat che do lam lanh
						Vac_on;
						Root_on; //Chay root pump
						Pump2_on;//ngat bom dau ra	
						HAL_Delay(1000);
						Pump1_on; //Chay bom dau vao
						if((flow>0)&&(flow<2000)&&(setTemp>=temp)){
							Heater1_on;
							Heater2_off;
						}
						if((flow>2000)&&(setTemp>=temp)) {
							Heater1_on;
							Heater2_on;
						}
						if((flow==0)||(setTemp<temp)){
							Heater1_off;
							Heater2_off;
						}
						Transfer_data_IT();//Cap nhat trang thai ket thuc
						
						stage=100;//Out khoi vong lap
					}
					if(stage==4){ //Tu on dinh xuong duoi muc thap
						Cooler_on; //Bat che do lam lanh
						Vac_on;
						Root_on; //Chay root pump
						Pump2_off;//ngat bom dau ra	
						HAL_Delay(1000);
						Pump1_on; //Chay bom dau vao
						if((flow>0)&&(flow<2000)&&(setTemp>=temp)){
							Heater1_on;
							Heater2_off;
						}
						if((flow>2000)&&(setTemp>=temp)) {
							Heater1_on;
							Heater2_on;
						}
						if((flow==0)||(setTemp<temp)){
							Heater1_off;
							Heater2_off;
						}
						Transfer_data_IT();//Cap nhat trang thai ket thuc
						stage=100;//Out khoi vong lap
					}

					else if(stage==100){;}
					
					
			}
			else {//Che do manual
				// Ngat tat ca cac thiet bi
				if(stage==50){


					Transfer_data_IT();//Cap nhat trang thai ket thuc
					stage=100;//Out khoi vong lap
					}
				if(stage==100){;}
				
			}
			
	}

			else {
				if(stage!=100){
					// Ngat tat ca cac thiet bi
					Pump1_off;
					Pump2_off;
					Root_off;
					Vac_off;
					Heater1_off;
					Heater2_off;
					VE_off;
					Cooler_off;			
					Transfer_data_IT();//Cap nhat trang thai ket thuc
					stage=100;//Out khoi vong lap
				}
				else{}
			}
			

			HAL_Delay(200);


  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  } //end cua if start
	else { //Neu chuyen sang che do tam dung
		if(stage!=100){
			HAL_GPIO_WritePin(led_GPIO_Port,led_Pin,GPIO_PIN_SET);
			Pump1_off;
			Pump2_off;
			Root_off;
			Vac_off;
			Heater1_off;
			Heater2_off;
			VE_off;
			Cooler_off;			
			Transfer_data_IT();//Cap nhat trang thai ket thuc
			stage=100;
		}
		}
	if(flow_check!=flow){
		flow_check = flow;
		Transfer_data_IT();
	
	}
	if(temp_check!=temp){
		temp_check = temp ;
		Transfer_data_IT();
	
	}
	if(pressure_check!=pressure){
		pressure_check = pressure ;
		Transfer_data_IT();
	
	}
	if(setTemp_check!=setTemp){
		setTemp_check = setTemp ;
		Transfer_data_IT();
	
	}
		HAL_Delay(200);

} //end cua vong while(1)
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 100;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_ETRMODE2;
  sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;
  sClockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
  sClockSourceConfig.ClockFilter = 0;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM4 init function */
static void MX_TIM4_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 36000;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 2000;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Pump1_Pin|Pump2_Pin|Heater2_Pin|Heater1_Pin 
                          |VE_Pin|Cooler_Pin|Vac_Pin|Root_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : led_Pin */
  GPIO_InitStruct.Pin = led_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(led_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PC14 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LOW_SENSOR_Pin HIGH_SENSOR_Pin BUBBLE_SENSOR_Pin */
  GPIO_InitStruct.Pin = LOW_SENSOR_Pin|HIGH_SENSOR_Pin|BUBBLE_SENSOR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI1_CS_Pin */
  GPIO_InitStruct.Pin = SPI1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI1_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Pump1_Pin Pump2_Pin Heater2_Pin Heater1_Pin 
                           VE_Pin Cooler_Pin Vac_Pin Root_Pin */
  GPIO_InitStruct.Pin = Pump1_Pin|Pump2_Pin|Heater2_Pin|Heater1_Pin 
                          |VE_Pin|Cooler_Pin|Vac_Pin|Root_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

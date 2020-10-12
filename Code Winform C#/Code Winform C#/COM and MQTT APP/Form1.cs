using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;
using MetroFramework;
using Newtonsoft.Json.Linq;
using Microsoft.Office;
using Microsoft.Office.Interop.Excel;
using System.Reflection;
using System.Runtime.InteropServices;
using Application = Microsoft.Office.Interop.Excel._Application;
// Giao tiếp qua Serial
using System.IO;
using System.IO.Ports;
using System.Xml;
// Thêm ZedGraph
using ZedGraph;
//Thêm live chart
using LiveCharts; //Core of the library
using LiveCharts.Wpf; //The WPF controls
using LiveCharts.Defaults;
using LiveCharts.WinForms; //the WinForm wrappers
using rtChart;
using SymbolFactoryDotNet;

namespace bunifuapp
{

    public partial class Form1 : Form
    {
        string mqtt_json;
        bool mode=false;
        int tick1 = 0; // Bien thoi gian
        int key_start = 1, key_mode = 0, key_LS = 1, key_HS = 1, key_BS = 1, key_P1 = 0, key_P2 = 0, key_R = 0, key_V = 0, key_C = 0, key_VE = 0, key_H1 = 0, key_H2 = 0, key_flow = 0, key_temp = 0, key_setTemp = 0, key_pressure = 0; //0->65535
        int key_start_ct=1, key_mode_ct = 0, key_P1_ct = 0, key_P2_ct = 0, key_R_ct = 0, key_V_ct = 0, key_C_ct = 0, key_VE_ct = 0, key_H1_ct = 0, key_H2_ct = 0, key_flow_ct = 0, key_temp_ct = 0,key_pressure_ct=0;
        //string esp_sub="hello";
        string esp_sub;
        int[] esp_sub_buffer = new int[10];

        private void tb_setTemp_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                key_setTemp= Convert.ToInt32(tb_setTemp.Text);
                Console.WriteLine("key_setTemp_ct:" + tb_setTemp.Text);
                client.Publish("ESP_SUB", Encoding.UTF8.GetBytes("^" + tb_setTemp.Text + "\n"));
                if (key_temp>key_setTemp)
                {
                    lb_quanhiet.Visible = true;
                }
                else if (key_temp <= key_setTemp)
                {
                    lb_quanhiet.Visible = false;
                }


            }

        }



        //char[] esp_sub = new char[3];

        private void bt_H2_Click(object sender, EventArgs e)
        {
            if (key_H2_ct == 0)
            {
                key_H2_ct = 1;
                bt_H2.IdleFillColor = Color.SeaGreen;
                bt_H2.ButtonText = "ON";
            }
            else
            {
                key_H2_ct = 0;
                bt_H2.IdleFillColor = Color.OrangeRed;
                bt_H2.ButtonText = "OFF";
            }
            transfer_data();
            Console.WriteLine("H2:" + bt_H2.ButtonText);

        }





        private void bt_VE_Click(object sender, EventArgs e)
        {
            if (key_VE_ct == 0)
            {
                key_VE_ct = 1;
                bt_VE.IdleFillColor = Color.SeaGreen;
                bt_VE.ButtonText = "ON";
            }
            else
            {
                key_VE_ct = 0;
                bt_VE.IdleFillColor = Color.OrangeRed;
                bt_VE.ButtonText = "OFF";
            }
            transfer_data();
            Console.WriteLine("VE:" + bt_VE.ButtonText);
        }

        private void bt_C_Click(object sender, EventArgs e)
        {
            if (key_C_ct == 0)
            {
                key_C_ct = 1;
                bt_C.IdleFillColor = Color.SeaGreen;
                bt_C.ButtonText = "ON";
            }
            else
            {
                key_C_ct = 0;
                bt_C.IdleFillColor = Color.OrangeRed;
                bt_C.ButtonText = "OFF";
            }
            transfer_data();
            Console.WriteLine("C:" + bt_C.ButtonText);
        }

        private void bt_P1_Click(object sender, EventArgs e)
        {
            if (key_P1_ct == 0)
            {
                key_P1_ct = 1;
                bt_P1.IdleFillColor = Color.SeaGreen;
                bt_P1.ButtonText = "ON";
            }
            else
            {
                key_P1_ct = 0;
                bt_P1.IdleFillColor = Color.OrangeRed;
                bt_P1.ButtonText = "OFF";
            }
            transfer_data();
            Console.WriteLine("P1:" + bt_P1.ButtonText);
        }

        private void bt_R_Click(object sender, EventArgs e)
        {
            if (key_R_ct == 0)
            {
                key_R_ct = 1;
                bt_R.IdleFillColor = Color.SeaGreen;
                bt_R.ButtonText = "ON";
            }
            else
            {
                key_R_ct = 0;
                bt_R.IdleFillColor = Color.OrangeRed;
                bt_R.ButtonText = "OFF";
            }
            transfer_data();
            Console.WriteLine("R:" + bt_R.ButtonText);
        }

        private void bt_V_Click(object sender, EventArgs e)
        {
            if (key_V_ct == 0)
            {
                key_V_ct = 1;
                bt_V.IdleFillColor = Color.SeaGreen;
                bt_V.ButtonText = "ON";
            }
            else
            {
                key_V_ct = 0;
                bt_V.IdleFillColor = Color.OrangeRed;
                bt_V.ButtonText = "OFF";
            }
            transfer_data();
            Console.WriteLine("V:" + bt_V.ButtonText);
        }

        private void bt_H1_Click(object sender, EventArgs e)
        {
            if (key_H1_ct == 0)
            {
                key_H1_ct = 1;
                bt_H1.IdleFillColor = Color.SeaGreen;
                bt_H1.ButtonText = "ON";
            }
            else
            {
                key_H1_ct = 0;
                bt_H1.IdleFillColor = Color.OrangeRed;
                bt_H1.ButtonText = "OFF";
            }
            transfer_data();
            Console.WriteLine("H1:" + bt_H1.ButtonText);

        }

        private void bt_P2_Click(object sender, EventArgs e)
        {
            if (key_P2_ct == 0)
            {
                key_P2_ct = 1;
                bt_P2.IdleFillColor = Color.SeaGreen;
                bt_P2.ButtonText = "ON";
            }
            else
            {
                key_P2_ct = 0;
                bt_P2.IdleFillColor = Color.OrangeRed;
                bt_P2.ButtonText = "OFF";
            }
            transfer_data();
            Console.WriteLine("P2:" + bt_P2.ButtonText);
        }


        private void bt_start_Click(object sender, EventArgs e)
        {
            if (key_start_ct == 0)
            {
                key_start_ct = 1;
            }
            else
            {
                key_start_ct = 0;
            }
            transfer_data();
            Console.WriteLine("Start:" + bt_start.ButtonText);
        }

        private void bt_mode_Click(object sender, EventArgs e)
        {
            if (key_start == 0) 
            {

                if (key_mode_ct == 0)
                {
                    key_mode_ct = 1;

                }
                else
                {
                    key_mode_ct = 0;

                }
                transfer_data();
                Console.WriteLine("mode:" + bt_mode.ButtonText);
            }


        }

        private void tb_pressure_sim_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                key_pressure_ct = Convert.ToInt32(tb_pressure_sim.Text);
                cir_pressure.Value = key_pressure_ct;
                Console.WriteLine("key_pressure_ct:" + tb_pressure_sim.Text);
                client.Publish("ESP_SUB", Encoding.UTF8.GetBytes("%" + tb_pressure_sim.Text + "\n"));

            }
        }

        private void tb_temp_sim_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                key_temp_ct = Convert.ToInt32(tb_temp_sim.Text);
                cir_temp.Value = key_temp_ct;
                Console.WriteLine("key_temp_ct:" + tb_temp_sim.Text);
                client.Publish("ESP_SUB", Encoding.UTF8.GetBytes("$" + tb_temp_sim.Text + "\n"));
            }
        }

        private void tb_flow_sim_KeyDown(object sender, KeyEventArgs e)
        {
            if(e.KeyCode == Keys.Enter)
            {
                key_flow_ct = Convert.ToInt32(tb_flow_sim.Text);
                cir_flow.Value = key_flow_ct;
                Console.WriteLine("key_flow_ct:" + tb_flow_sim.Text);
                client.Publish("ESP_SUB", Encoding.UTF8.GetBytes("#" + tb_flow_sim.Text + "\n"));
            }
        }



        //string[] esp_sub = new string[11];
        

        private void transfer_data()
        {
            //int key_start_ct = 1, key_mode_ct = 1, key_P1_ct = 0, key_P2_ct = 0, key_R_ct = 0, key_V_ct = 0, key_C_ct = 0, 
            //key_VE_ct = 0, key_H1_ct = 0, key_H2_ct = 0, key_flow_ct = 0, key_temp_ct = 0, key_pressure_ct = 0;
            esp_sub_buffer[0] = key_start_ct;
            esp_sub_buffer[1] = key_mode_ct;
            esp_sub_buffer[2] = key_P1_ct;
            esp_sub_buffer[3] = key_P2_ct;
            esp_sub_buffer[4] = key_R_ct;
            esp_sub_buffer[5] = key_V_ct;
            esp_sub_buffer[6] = key_C_ct;
            esp_sub_buffer[7] = key_VE_ct;
            esp_sub_buffer[8] = key_H1_ct;
            esp_sub_buffer[9] = key_H2_ct;
            //esp_sub_buffer[10] = key_flow_ct;
            //esp_sub_buffer[11] = key_temp_ct;
            esp_sub = String.Join(",",esp_sub_buffer.Select(p => p.ToString()).ToArray());
            Console.WriteLine("esp_sub:" + esp_sub);
            client.Publish("ESP_SUB", Encoding.UTF8.GetBytes("."+esp_sub + "\n"));
            //client.Publish("ESP_SUB", Encoding.UTF8.GetBytes("@xinchao\n"));


            //esp_sub[1] = (char)key_mode_ct;
            //esp_sub[2] = (char)Keys.Enter;

            //client.Publish("ESP_SUB", Encoding.UTF8.GetBytes(esp_sub[0]));
            //test[0] = char.Parse(key_mode_ct);
            //client.Publish("ESP_SUB", Encoding.UTF8.GetBytes(test);

        }


        private void btn_ConnectMQTT_Click(object sender, EventArgs e)
        {
            try
            {
                client = new MqttClient(txb_BrokerName.Text, int.Parse(txb_Port.Text), false, MqttSslProtocols.None, null, null);
                client.ProtocolVersion = MqttProtocolVersion.Version_3_1;
                byte code = client.Connect(Guid.NewGuid().ToString(), txb_UserName.Text, txb_Password.Text);
                if (code == 0)
                {
                    MetroMessageBox.Show(this, "Đã kết nối tới MQTT Cloud", "Connected", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    Status_mqtt.Checked = true;
                    //Subcribe Topic
                    client.MqttMsgPublishReceived += client_MqttMsgPublishReceived;
                    client.Subscribe(new string[] { "ESP_SUB/T1", "ESP_SUB/T2", "test", "ESP_PUB" }, new byte[] { 0, 0, 0, 0 });
                    //READ LED STATUS
                    client.Publish("ESP_SUB", Encoding.UTF8.GetBytes("@Client Connected\n"));

                }

                else MetroMessageBox.Show(this, "Connect Fail", "Fail", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            catch (Exception)
            {
                MetroMessageBox.Show(this, "Wrong Format", "Wrong", MessageBoxButtons.OK, MessageBoxIcon.Question);

            }
        }

        private void btn_DisconnectMQTT_Click(object sender, EventArgs e)
        {
            
            if (client.IsConnected)
            {
                client.Publish("ESP_SUB", Encoding.UTF8.GetBytes("@Client Disconnected\n"));
                Status_mqtt.Checked = false;
                MetroMessageBox.Show(this, "Ngắt kết nối tới MQTT Cloud", "Disconnected", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                client.Disconnect();
            }
        }

        public Form1()
        {
            InitializeComponent();

        }
        static MqttClient client;

        //Hàm SetText khi dữ liệu xử lý khác thread
        kayChart SerialDataChart;

      
        private void Form1_Load_1(object sender, EventArgs e)
        {
            txt_time.Text = DateTime.Now.ToLongTimeString();
            txt_date.Text = DateTime.Now.ToLongDateString();
            txtkq.Visible = true;
            txt_dong1.Visible = false;
            txt_dong2.Visible = false;
            MQTT_PANEL.Visible = true;
            panel_main.Visible = true;
            tb_send.Visible = false;
            timer1.Interval = 1000;
            // khai báo MQTT
            txb_BrokerName.Text = "tailor.cloudmqtt.com";
            txb_Port.Text = "16370";
            txb_UserName.Text = "hiepbk";
            txb_Password.Text = "8286597";
            Status_mqtt.Checked = false;
            toolStripStatusLabel1.Text = "Giám sát máy lọc dầu";
            lb_muccao.Visible = false;
            lb_quanhiet.Visible = false;
            lb_apsuat.Visible = false;
            bt_C.Visible = false;
            bt_H1.Visible = false;
            bt_H2.Visible = false;
            bt_P1.Visible = false;
            bt_P2.Visible = false;
            bt_V.Visible = false;
            bt_VE.Visible = false;
            bt_R.Visible = false;
            tb_setTemp.Text = key_setTemp.ToString();
            tb_temp.Text = key_temp.ToString();
            tb_flow.Text = key_flow.ToString();
            tb_pressure.Text = key_pressure.ToString();
            tb_flow_sim.Text = key_flow_ct.ToString();
            tb_temp_sim.Text = key_temp_ct.ToString();
            tb_pressure_sim.Text = key_pressure_ct.ToString();




        }
        // Hàm Tick 
        private void timer1_Tick(object sender, EventArgs e)
        {
            txt_time.Text = DateTime.Now.ToLongTimeString();
            txt_date.Text = DateTime.Now.ToLongDateString();
            timer1.Start();
            tick1++;

        }


        Action<string, string> ReceiveAction;

        void client_MqttMsgPublishReceived(object sender, MqttMsgPublishEventArgs e)
        {
            ReceiveAction = Receive;
            try
            {
                this.BeginInvoke(ReceiveAction, Encoding.UTF8.GetString(e.Message), e.Topic);
            }
            catch { };
        }
        void Receive(string message, string topic)
        {
            if (topic == "ESP_PUB")
            {

                if (mode == false) {

                    mqtt_json = message;
                    try
                    {
                        JObject wifi_json = JObject.Parse(mqtt_json);// Chuyen chuoi string msg sang dang json
                        Console.WriteLine("wifi_json: " + wifi_json);


                        string chuoi_json = wifi_json.ToString();


                        if (chuoi_json[6] == 's')
                        {
                            txt_dong1.Text = chuoi_json;
                            Console.WriteLine("mqtt_gui_ve: " + chuoi_json);
                            // Lấy giá trị ID
                            key_start = (int)wifi_json["s"];
                            key_mode = (int)wifi_json["m"];
                            key_LS = (int)wifi_json["LS"];
                            key_HS = (int)wifi_json["HS"];
                            key_BS = (int)wifi_json["BS"];
                            key_P1 = (int)wifi_json["P1"];
                            key_P2 = (int)wifi_json["P2"];
                            key_pressure = (int)wifi_json["press"];
                            key_setTemp = (int)wifi_json["setTemp"];

                            //Lấy giá trị U2
                            //key_HS = (string)wifi_json["U2"];
                            //txb_U2.Text = key_HS;
                            //U2 = float.Parse(key_HS);

                        }
                        else
                        {
                            txt_dong2.Text = chuoi_json;
                            Console.WriteLine("mqtt_gui_ve: " + chuoi_json);
                            key_R = (int)wifi_json["R"];
                            key_V = (int)wifi_json["V"];
                            key_C = (int)wifi_json["C"];
                            key_VE = (int)wifi_json["VE"];
                            key_H1 = (int)wifi_json["H1"];
                            key_H2 = (int)wifi_json["H2"];
                            key_flow = (int)wifi_json["flow"];
                            key_temp = (int)wifi_json["temp"];
                            Console.WriteLine("flow:"+key_flow +"temp:"+key_temp);
                            //txtkq_2.Text = chuoi_json;
                            //// Lấy giá trị S
                            //key_V = (string)wifi_json["csS"];
                            //txb_S.Text = key_V;
                            //S = float.Parse(key_V); // chuyen sang bien so thuc

                        }

                    }
                    catch { }

                   


                    //{ key1: [ '1', '0','1','0' ],key2:['1','0','1'] ,key2:['26','50']}


                }
                



           }
            
            sensor_object(sym_LS, key_LS);
            sensor_object(sym_HS, key_HS);
            sensor_object(sym_BS, key_BS);
            control_object(sym_Pump1, key_P1);
            control_object(sym_Pump2, key_P2);
            control_object(sym_Root, key_R);
            control_object(sym_Vac, key_V);
            control_object(sym_Cooler, key_C);
            control_object(sym_VE, key_VE);
            control_object(sym_Heater1, key_H1);
            control_object(sym_Heater2, key_H2);
            control_object(sym_condenser, key_C);
            handle_data();


        }
        private void control_object(StandardControl animate_sym, int Value)
        {
            if (Value == 0)
            {
                animate_sym.DiscreteValue1 = true;
                animate_sym.DiscreteValue2 = false;
            }
            else
            {
                animate_sym.DiscreteValue1 = false;
                animate_sym.DiscreteValue2 = true;
            }

        }
        private void sensor_object(StandardControl animate_sym, int Value)
        {
            if (Value == 1)
            {
                animate_sym.DiscreteValue1 = true;
                animate_sym.DiscreteValue2 = false;
            }
            else
            {
                animate_sym.DiscreteValue1 = false;
                animate_sym.DiscreteValue2 = true;
            }

        }
        private void handle_data()
        {
            tb_flow.Text = Convert.ToString(key_flow);
            tb_temp.Text = Convert.ToString(key_temp);
            tb_pressure.Text = Convert.ToString(key_pressure);
            tb_setTemp.Text = Convert.ToString(key_setTemp);
            try { key_setTemp = Convert.ToInt32(tb_setTemp.Text); }
            catch { }
            
            if (key_temp > key_setTemp)
            {
                lb_quanhiet.Visible = true;

            }
            else if(key_temp <= key_setTemp)
            {
                lb_quanhiet.Visible = false;
            }

            if(key_HS == 0)
            {
                lb_muccao.Visible = true;
            }
            else if (key_HS == 1)
            {
                lb_muccao.Visible = false;
            }
            if (key_pressure > 4)
            {
                lb_apsuat.Visible=true;
            }
            else if (key_pressure <= 4)
            {
                lb_apsuat.Visible = false;
            }
            if (key_start==1)
            {
                bt_start.ButtonText = "START";
                bt_start.IdleFillColor = Color.Green;
                lb_start.Text = "HỆ THỐNG ĐANG CHẠY";
                lb_start.BackColor = Color.Green;
            }
            else if (key_start == 0)
            {
                bt_start.ButtonText = "STOP";
                bt_start.IdleFillColor = Color.OrangeRed;
                lb_start.Text = "HỆ THỐNG ĐANG DỪNG";
                lb_start.BackColor = Color.OrangeRed;
            }
            if (key_mode == 1)
            {
                bt_mode.ButtonText = "AUTO";
                bt_mode.IdleFillColor = Color.Green;
                lb_mode.Text = "TỰ ĐỘNG";
                lb_mode.BackColor = Color.Green;
                bt_P1.Visible = false;
                bt_P2.Visible = false;
                bt_VE.Visible = false;
                bt_C.Visible = false;
                bt_R.Visible = false;
                bt_V.Visible = false;
                bt_H1.Visible = false;
                bt_H2.Visible = false;

            }
            else if (key_mode == 0)
            {
                bt_mode.ButtonText = "MANUAL";
                bt_mode.IdleFillColor = Color.OrangeRed;
                lb_mode.Text = "TÙY CHỈNH";
                lb_mode.BackColor = Color.OrangeRed;
                bt_P1.Visible = true;
                bt_P2.Visible = true;
                bt_VE.Visible = true;
                bt_C.Visible = true;
                bt_R.Visible = true;
                bt_V.Visible = true;
                bt_H1.Visible = true;
                bt_H2.Visible = true;
            }
            if (key_P1 == 1)
            {
                lb_P1.Text = "PUMP1_ON";
                lb_P1.BackColor = Color.Green;
            }
            else if (key_P1 == 0)
            {
                lb_P1.Text = "PUMP1_OFF";
                lb_P1.BackColor = Color.OrangeRed;
            }
            if (key_P2 == 1)
            {
                lb_P2.Text = "PUMP1_ON";
                lb_P2.BackColor = Color.Green;
            }
            else if (key_P2 == 0)
            {
                lb_P2.Text = "PUMP2_OFF";
                lb_P2.BackColor = Color.OrangeRed;
            }
            if (key_R == 1)
            {
                lb_R.Text = "ROOT_ON";
                lb_R.BackColor = Color.Green;
            }
            else if (key_R == 0)
            {
                lb_R.Text = "ROOT_OFF";
                lb_R.BackColor = Color.OrangeRed;
            }
            if (key_V == 1)
            {
                lb_V.Text = "VAC_ON";
                lb_V.BackColor = Color.Green;
            }
            else if (key_V == 0)
            {
                lb_V.Text = "VAC_OFF";
                lb_V.BackColor = Color.OrangeRed;
            }
            if (key_C == 1)
            {
                lb_C.Text = "COOLER_ON";
                lb_C.BackColor = Color.Green;
            }
            else if (key_C == 0)
            {
                lb_C.Text = "COOLER_OFF";
                lb_C.BackColor = Color.OrangeRed;
            }
            if (key_VE == 1)
            {
                lb_VE.Text = "VE_ON";
                lb_VE.BackColor = Color.Green;
            }
            else if (key_VE == 0)
            {
                lb_VE.Text = "VE_OFF";
                lb_VE.BackColor = Color.OrangeRed;
            }
            if (key_H1 == 1)
            {
                lb_H1.Text = "HEATER1_ON";
                lb_H1.BackColor = Color.Green;
            }
            else if (key_H1 == 0)
            {
                lb_H1.Text = "HEATER1_OFF";
                lb_H1.BackColor = Color.OrangeRed;
            }
            if (key_H2 == 1)
            {
                lb_H2.Text = "HEATER2_ON";
                lb_H2.BackColor = Color.Green;
            }
            else if (key_H2 == 0)
            {
                lb_H2.Text = "HEATER2_OFF";
                lb_H2.BackColor = Color.OrangeRed;
            }
        }



    







    }
}

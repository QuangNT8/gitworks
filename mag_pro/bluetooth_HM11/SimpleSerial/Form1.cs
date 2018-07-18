using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;

// Thêm 3 em này vào là OK, để sài SerialPort
using System.IO;
using System.IO.Ports;
using System.Xml;

namespace SimpleSerial
{
    public partial class Form1 : Form
    {
        // Add this variable 
        string RxString;
        int exit = 0;
        int mode = 0;
        int count_int = 0;
        int lencmd = 0;
        bool conti_sending=false;
        int numchar = 0;
        string cmdneed2send;
        string mulcmdneed2send;
        public Form1()
        {
            InitializeComponent();
            string[] ports = SerialPort.GetPortNames();
            PORT_LS.Items.AddRange(ports);
        }

        

        private void buttonStart_Click(object sender, EventArgs e)
        {
            RxString = null;
            char[] buf;
            mode = 1;
            serialPort1.PortName = PORT_LS.Text;//"COM4";
            serialPort1.DataBits = 8;
            serialPort1.BaudRate = 9600;
          

            serialPort1.Open();
            if (serialPort1.IsOpen)
            {
                //serialPort1.Write(new byte[] { 101 }, 0, 1);
                serialPort1.Write(new byte[] { 13, 10 }, 0, 2);
                buttonStart.Enabled = false;

                textBox1.ReadOnly = false;
                Save_BT.Enabled = true;
                connect_bt.Enabled = true;
                //Download_BT.Enabled = true;
                //Format_BT.Enabled = true;
                //update_BT.Enabled = true;
                //login_cmd();
            }

            //timer2.Interval = 2000;
            //timer2.Start();
        }


        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {

            if (serialPort1.IsOpen)
            {
                serialPort1.Close();
            }
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            // If the port is closed, don't try to send a character.
            if (!serialPort1.IsOpen) return;

            // If the port is Open, declare a char[] array with one element.
            char[] buff = new char[1];

            // Load element 0 with the key character.
            buff[0] = e.KeyChar;

            // Send the one character buffer.
            serialPort1.Write(buff, 0, 1);

            // Set the KeyPress event as handled so the character won't
            // display locally. If you want it to display, omit the next line.
            e.Handled = true;
        }

        private void DisplayText(object sender, EventArgs e)
        {
            //if (pass_mode != 0) 
            textBox1.AppendText(RxString);
            //else PASSWORD.AppendText(RxString);
        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {

            RxString = serialPort1.ReadExisting();
            this.Invoke(new EventHandler(DisplayText));
            System.Diagnostics.Debug.WriteLine(RxString);
        }


        delegate void EnableDownloadButtonCallBack();


        private void Form1_Load(object sender, EventArgs e)
        {
            PORT_LS.SelectedIndex = 0;
            connect_bt.Enabled = false;
            Save_BT.Enabled = false;
            textBox1.Enabled = true;
            textBox1.ReadOnly = true;
            //update_BT.Enabled = false;
        }


        private void button1_Click(object sender, EventArgs e)
        {
            PORT_LS.Items.Clear();
            string[] ports = SerialPort.GetPortNames();
            PORT_LS.Items.AddRange(ports);
        }

        //===this timer use for check response==============


        private void button2_Click_1(object sender, EventArgs e)
        {
            saveFileDialog1.Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                File.WriteAllText(saveFileDialog1.FileName, textBox1.Text);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            Stream myStream = null;
            openFileDialog1.InitialDirectory = "c:\\";
            openFileDialog1.Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
            openFileDialog1.FilterIndex = 2;
            openFileDialog1.RestoreDirectory = true;
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                if ((myStream = openFileDialog1.OpenFile()) != null)
                {
                    string strfilename = openFileDialog1.FileName;
                    textBox1.Text = File.ReadAllText(strfilename);
                }
            }
        }

        private void showclk_bt_Click(object sender, EventArgs e)
        {
            textBox1.Text = "";
            mulcmdneed2send = "c/ ";//textBox2.Text+" ";
            sendmulticmd();         
        }


        private void PASSWORD_KeyPress_1(object sender, KeyPressEventArgs e)
        {
            // If the port is closed, don't try to send a character.
            if (!serialPort1.IsOpen) return;

            // If the port is Open, declare a char[] array with one element.
            char[] buff = new char[1];

            // Load element 0 with the key character.
            buff[0] = e.KeyChar;

            // Send the one character buffer.
            serialPort1.Write(buff, 0, 1);

            // Set the KeyPress event as handled so the character won't
            // display locally. If you want it to display, omit the next line.
            e.Handled = true;
        }


       
        private void sendmulticmd()
        {
            numchar = 0;
            lencmd = mulcmdneed2send.Length;
            sendcmd(mulcmdneed2send[numchar].ToString());
        }

        private void sendcmd(string incmd)
        {
            cmdneed2send = incmd;
            serialPort1.Write(cmdneed2send.ToCharArray(), 0, 1);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            textBox1.Text = "";
        }

        private void connect_bt_Click(object sender, EventArgs e)
        {
            sendcmd("A");
            sendcmd("T");
            sendcmd("+");
            sendcmd("D");
            sendcmd("I");
            sendcmd("S");
            sendcmd("C");
            sendcmd("?");
            timer4.Interval = 6000;
            timer4.Start();
            connect_bt.Enabled = false;
        }
        private void timer4_Tick(object sender, EventArgs e)
        {
            sendcmd("A");
            sendcmd("T");
            sendcmd("+");
            sendcmd("C");
            sendcmd("O");
            sendcmd("N");
            sendcmd("N");
            sendcmd("0");
            timer5.Interval = 2000;
            timer5.Start();
            timer4.Stop();
        }

        private void timer5_Tick(object sender, EventArgs e)
        {
            textBox1.Text = "ready";
            sendcmd(" ");
            serialPort1.Write(new byte[] { 13, 10 }, 0, 2);
            sendcmd(" ");
            serialPort1.Write(new byte[] { 13, 10 }, 0, 2);
            sendcmd(" ");
            serialPort1.Write(new byte[] { 13, 10 }, 0, 2);
            timer5.Stop();
            connect_bt.Enabled = false;
        }
    }
}
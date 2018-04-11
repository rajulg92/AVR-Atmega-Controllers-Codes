using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.IO;
using System.IO.Ports;

namespace ArduinoC3
{
    public partial class Form1 : Form
    {
        SerialPort port;
        string comPort;
        string indata;
        public Form1()
        {
            try
            {
                InitializeComponent();
                comPort = "COM23"; 
                port = new SerialPort(comPort, 9600, Parity.None, 8, StopBits.One);
                port.DtrEnable = true;
                port.Open();
                port.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
            }
            catch (Exception /*ex*/)
            {
                MessageBox.Show( "Can't Open COM23"); 
            }
            if (port.IsOpen) 
            {
                buttonSend.Enabled = true;
                textBoxSend.ReadOnly = false;
            }
            else
            {
                buttonSend.Enabled = false;
                textBoxSend.ReadOnly = true;
            }
        }

        private void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            indata = sp.ReadExisting();
        }


        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            if (!port.IsOpen) 
                return;
            else 
            {
                port.Write(textBoxSend.Text);
                textBoxSend.Text = "";
            }
            textBoxReceive.Text = indata; 
        }
    }
}

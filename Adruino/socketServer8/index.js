const PORT = 3484;									//Đặt địa chỉ Port được mở ra để tạo ra chương trình mạng Socket Server

var http = require('http');
var express = require('express');							//#include thư viện express - dùng để tạo server http nhanh hơn thư viện http cũ
var socketio = require('socket.io')				//#include thư viện socketio

var ip = require('ip');
var app = express();									//#Khởi tạo một chương trình mạng (app)
var server = http.Server(app)

var io = socketio(server);								//#Phải khởi tạo io sau khi tạo app

var webapp_nsp = io.of('/webapp')				//namespace của webapp
var esp8266_nsp = io.of('/esp8266')				//namespace của esp8266

var middleware = require('socketio-wildcard')();		//Để có thể bắt toàn bộ lệnh!
esp8266_nsp.use(middleware);									//Khi esp8266 emit bất kỳ lệnh gì lên thì sẽ bị bắt
webapp_nsp.use(middleware);									//Khi webapp emit bất kỳ lệnh gì lên thì sẽ bị bắt

server.listen(PORT);										// Cho socket server (chương trình mạng) lắng nghe ở port 3484
console.log("Server nodejs chay tai dia chi: " + ip.address() + ":" + PORT)

//Cài đặt webapp các fie dữ liệu tĩnh
app.use(express.static("node_modules/mobile-angular-ui")) 			// Có thể truy cập các file trong node_modules/mobile-angular-ui từ xa
app.use(express.static("node_modules/angular")) 							// Có thể truy cập các file trong node_modules/angular từ xa
app.use(express.static("node_modules/angular-route")) 				// Có thể truy cập các file trong node_modules/angular-route từ xa
app.use(express.static("node_modules/socket.io-client")) 				// Có thể truy cập các file trong node_modules/socket.io-client từ xa
app.use(express.static("node_modules/angular-socket-io"))			// Có thể truy cập các file trong node_modules/angular-socket-io từ xa
app.use(express.static("webapp")) 													// Dùng để lưu trữ webapp


//giải nén chuỗi JSON thành các OBJECT
function ParseJson(jsondata) {
    try {
        return JSON.parse(jsondata);
    } catch (error) {
        return null;
    }
}


//Bắt các sự kiện khi esp8266 kết nối
esp8266_nsp.on('connection', function(socket) {
	console.log('esp8266 connected...')
	
	socket.on('disconnect', function() {
		console.log("Disconnect socket esp8266")
	})
	
	//nhận được bất cứ lệnh nào
	socket.on("*", function(packet) {
		console.log("esp8266 rev and send to webapp packet: ", packet.data) //in ra để debug
		var eventName = packet.data[0]
		var eventJson = packet.data[1] || {} //nếu gửi thêm json thì lấy json từ lệnh gửi, không thì gửi chuỗi json rỗng, {}
		webapp_nsp.emit(eventName, eventJson) //gửi toàn bộ lệnh + json đến webapp
	})
})

//Bắt các sự kiện khi webapp kết nối

webapp_nsp.on('connection', function(socket) {
	
	console.log('webapp connected')
	
	//Khi webapp socket bị mất kết nối
	socket.on('disconnect', function() {
		console.log("Disconnect socket webapp")
	})
	
	socket.on('*', function(packet) {
		console.log("webapp rev and send to esp8266 packet: ", packet.data) //in ra để debug
		var eventName = packet.data[0]
		var eventJson = packet.data[1] || {} //nếu gửi thêm json thì lấy json từ lệnh gửi, không thì gửi chuỗi json rỗng, {}
		esp8266_nsp.emit(eventName, eventJson) //gửi toàn bộ lệnh + json đến esp8266
	});
})
/*
//Khi có mệt kết nối được tạo giữa Socket Client và Socket Server
io.on('connection', function(socket) {	
	//hàm console.log giống như hàm Serial.println trên Arduino
    console.log("Connected"); //In ra màn hình console là đã có một Socket Client kết nối thành công.
	
	var led = [1, 0] //định nghĩa một mảng 1 chiều có 2 phần tử: true, false. Mảng này sẽ được gửi đi nhằm thay đổi sự sáng tắt của 2 con đèn LED đỏ và xanh. Dựa vào cài đặt ở Arduino mà đèn LEd sẽ bị bật hoặc tắt. Hãy thử tăng hoạt giảm số lượng biến của mảng led này xem. Và bạn sẽ hiểu điều kỳ diệu của JSON!
	
	//Tạo một chu kỳ nhiệm vụ sẽ chạy lại sau mỗi 200ms
	var interval1 = setInterval(function() {
		//đảo trạng thái của mảng led, đảo cho vui để ở Arduino nó nhấp nháy cho vui.
		for (var i = 0; i < led.length; i++) {
			led[i] = !led[i]
		}
		
		//Cài đặt chuỗi JSON, tên biến JSON này là json 
		var leddata = {
			"led": led //có một phần tử là "led", phần tử này chứa giá trị của mảng led.
		}
		socket.emit('LED1', leddata) //Gửi lệnh LED với các tham số của của chuỗi JSON
		console.log("blinking LED")//Ghi ra console.log là đã gửi lệnh LED
	}, 200)//200ms
	
	//Khi socket client bị mất kết nối thì chạy hàm sau.
	socket.on('disconnect', function() {
		console.log("disconnect") 	//in ra màn hình console cho vui
		clearInterval(interval1)		//xóa chu kỳ nhiệm vụ đi, chứ không xóa là cái task kia cứ chạy mãi thôi đó!
	})
});
//*/
#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <openssl/md5.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <iomanip>

using namespace std;

#define STOP_TEST 15 * 1024 * 1024
#define BUFFER_SIZE 21 * 1024 * 1024
#define CHUCK_SIZE 1024*10
typedef struct _CMD_PACKET {
   char HeadMarker;
   char Command;
   char Param1;
   char Param2;
   char TailMarker;
} CMD_PACKET, *PCMD_PACKET;




// Hàm gửi bản tin đến server
ssize_t sendPacketToServer(int client_socket, const CMD_PACKET &packet) {
   ssize_t bytes_sent = send(client_socket, &packet, sizeof(packet), 0);


   if (bytes_sent == -1) {
       std::cerr << "Gửi bản tin không thành công" << std::endl;
   } else {
       std::cout << "Đã gửi " << bytes_sent << " byte đến server" << std::endl;
   }
   return bytes_sent;
}


void computeMD5File(const char *filename, unsigned char *md5sum) {
   std::ifstream file(filename, std::ios::binary);
   if (!file.is_open()) {
       std::cerr << "Failed to open file for MD5 calculation." << std::endl;
       return;
   }


   MD5_CTX md5Context;
   MD5_Init(&md5Context);


   char buffer[50 * 1024];
   while (file.read(buffer, sizeof(buffer))) {
       MD5_Update(&md5Context, buffer, file.gcount());
   }


   MD5_Final(md5sum, &md5Context);


   file.close();
}


// NHAN ban tin 21mb
void receiveAndProcessData(int client_socket, const char *output_file) {
   char receive_buffer[CHUCK_SIZE];
   ssize_t totalReceived = 0;


   std::ofstream output_stream(output_file, std::ios::binary);
   if (!output_stream.is_open()) {
       std::cerr << "Không thể mở file đầu ra" << std::endl;
       close(client_socket);
       return;
   }


   while (true) {
       ssize_t bytes_received = recv(client_socket, receive_buffer, CHUCK_SIZE, 0);
       if(bytes_received <= 0) {


           break;


       }
       output_stream.write(receive_buffer, bytes_received);
       totalReceived += bytes_received;
       std::cout << "Received " << bytes_received << " bytes. Total received: " << totalReceived << std::endl;


       if (totalReceived == BUFFER_SIZE)
       {
           break;
          
       }
      
   }
  
   if(totalReceived == BUFFER_SIZE) {
       unsigned char md5sum[MD5_DIGEST_LENGTH];
       computeMD5File(output_file, md5sum);
       std::cout << "MD5 checksum: ";
       for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
           std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(md5sum[i]);
       }
       std::cout << std::endl;


   }
   output_stream.close();
}


void initializePacket(CMD_PACKET &packet, char head, char command, char param1, char param2, char tail) {
   packet.HeadMarker = head;
   packet.Command = command;
   packet.Param1 = param1;
   packet.Param2 = param2;
   packet.TailMarker = tail;
}
void receive5byte(int client_socket) {
   char buf[5];
   size_t bytes_rec = recv(client_socket, buf, 5, 0);


   if (bytes_rec == -1) {
       std::cout << "gửi bản tin không thành công" << std::endl;
       close(client_socket);
   } else {
       std::cout << "Nhận bản tin thành công. Dữ liệu nhận được: ";
       for (size_t i = 0; i < 5; ++i) {
           printf("%02X ", static_cast<unsigned char>(buf[i]));
       }
       std::cout << std::endl;
   }
}
int main() {
   const char *server_ip = "127.0.0.1";
   const int server_port = 1024;


   CMD_PACKET packet1;
   initializePacket(packet1, 0xAA, 0x3b, 0x2b, 0x55, 0x56);


   CMD_PACKET packet2;
   initializePacket(packet2, 0xAA, 0x3b, 0x2b, 0x55, 0x57);


   CMD_PACKET packet3;
   initializePacket(packet3, 0xAA, 0x5b, 0x2b, 0x55, 0x55);


   CMD_PACKET packet4;
   initializePacket(packet4, 0x1b, 0x58, 0x2b, 0x55, 0x55);


   int client_socket = socket(AF_INET, SOCK_STREAM, 0);
   if (client_socket == -1) {
       std::cerr << "Không thể tạo socket" << std::endl;
       return 1;
   }


   // Thiết lập thông tin địa chỉ của server
   sockaddr_in server_address;
   server_address.sin_family = AF_INET;
   server_address.sin_port = htons(server_port);
   if (inet_pton(AF_INET, server_ip, &server_address.sin_addr) <= 0) {
       std::cerr << "Không thể chuyển đổi địa chỉ IP" << std::endl;
       close(client_socket);
       return 1;
   }


   // Kết nối đến server
   if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
       std::cerr << "Không thể kết nối đến server" << std::endl;
       close(client_socket);
       return 1;
   }


   if (client_socket == -1) {
       return 1;
   }


   // gui tiep cac ban tin sai header
   ssize_t n_gui1 = sendPacketToServer(client_socket, packet1);


   ssize_t n_gui2 = sendPacketToServer(client_socket, packet2);


   ssize_t n_gui3 = sendPacketToServer(client_socket, packet3);


   // gui ban tin work nhung sai header
   ssize_t n_gui4 = sendPacketToServer(client_socket, packet4);

   return 0;
}



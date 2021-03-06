/******************************************************************************
* Copyright (c) 2000-2021 Ericsson Telecom AB
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v2.0
* which accompanies this distribution, and is available at
* https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*
* Contributors:
*  Peter Dimitrov- initial implementation and initial documentation
*  Adam Delic
*  Eduard Czimbalmos
*  Endre Kulcsar
*  Gabor Bettesch
*  Gabor Szalai
*  Tamas Buti
*  Zoltan Medve
******************************************************************************/
//
//  File:               SCTPasp_PT.hh
//  Description:        SCTPasp test port header
//  Prodnr:             CNL 113 469
// 


#ifndef SCTPasp__PT_HH
#define SCTPasp__PT_HH

#include <TTCN3.hh>
#include "SCTPasp_Types.hh"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>

namespace SCTPasp__Types {
  class ASP__SCTP;
  class ASP__SCTP__Connect;
  class ASP__SCTP__ConnectFrom;
  class ASP__SCTP__Listen;
  
  class SCTP__INIT;
  class SCTP__EVENTS;
  class SO__LINGER;
  class SCTP__RTOINFO;
  class SAC__STATE;
  class SPC__STATE;
  
  class ASP__SCTP__SetSocketOptions;
  class ASP__SCTP__Close;   
  class ASP__SCTP__ASSOC__CHANGE;
  class ASP__SCTP__PEER__ADDR__CHANGE;
  class ASP__SCTP__SEND__FAILED;
  class ASP__SCTP__REMOTE__ERROR;
  class ASP__SCTP__SHUTDOWN__EVENT;
  class ASP__SCTP__PARTIAL__DELIVERY__EVENT;
  class ASP__SCTP__ADAPTION__INDICATION;
  class ASP__SCTP__Connected;
  class ASP__SCTP__SENDMSG__ERROR;
  class ASP__SCTP__RESULT;
}

namespace SCTPasp__PortType {
class SCTPasp__PT_PROVIDER : public PORT {
public:
  SCTPasp__PT_PROVIDER(const char *par_port_name = NULL);
  ~SCTPasp__PT_PROVIDER();

  void set_parameter(const char *parameter_name,
    const char *parameter_value);

  void Handle_Fd_Event_Error(int my_fd);
  void Handle_Fd_Event_Writable(int my_fd);
  void Handle_Fd_Event_Readable(int my_fd);
  
protected:
  void user_map(const char *system_port);
  void user_unmap(const char *system_port);

  void user_start();
  void user_stop();

  void outgoing_send(const SCTPasp__Types::ASP__SCTP__Connect& send_par);
  void outgoing_send(const SCTPasp__Types::ASP__SCTP__ConnectFrom& send_par);
  void outgoing_send(const SCTPasp__Types::ASP__SCTP__Listen& send_par);
  void outgoing_send(const SCTPasp__Types::ASP__SCTP__SetSocketOptions& send_par);
  void outgoing_send(const SCTPasp__Types::ASP__SCTP__Close& send_par);
  void outgoing_send(const SCTPasp__Types::ASP__SCTP& send_par);

  virtual void incoming_message(const SCTPasp__Types::ASP__SCTP& incoming_par) = 0;
  virtual void incoming_message(const SCTPasp__Types::ASP__SCTP__ASSOC__CHANGE& incoming_par) = 0;
  virtual void incoming_message(const SCTPasp__Types::ASP__SCTP__PEER__ADDR__CHANGE& incoming_par) = 0;
  virtual void incoming_message(const SCTPasp__Types::ASP__SCTP__SEND__FAILED& incoming_par) = 0;
  virtual void incoming_message(const SCTPasp__Types::ASP__SCTP__REMOTE__ERROR& incoming_par) = 0;
  virtual void incoming_message(const SCTPasp__Types::ASP__SCTP__SHUTDOWN__EVENT& incoming_par) = 0;
  virtual void incoming_message(const SCTPasp__Types::ASP__SCTP__PARTIAL__DELIVERY__EVENT& incoming_par) = 0;
  virtual void incoming_message(const SCTPasp__Types::ASP__SCTP__ADAPTION__INDICATION& incoming_par) = 0;
  virtual void incoming_message(const SCTPasp__Types::ASP__SCTP__Connected& incoming_par) = 0;
  virtual void incoming_message(const SCTPasp__Types::ASP__SCTP__SENDMSG__ERROR& incoming_par) = 0;
  virtual void incoming_message(const SCTPasp__Types::ASP__SCTP__RESULT& incoming_par) = 0;

private:
  enum return_value_t { WHOLE_MESSAGE_RECEIVED, PARTIAL_RECEIVE, EOF_OR_ERROR };
  return_value_t getmsg(int fd, struct msghdr *msg);
  void handle_event(void *buf);
  void log(const char *fmt, ...);
  void error(const char *fmt, ...);
  void handle_event_reconnect(void *buf);
  void forced_reconnect(int attempts);
  // map operations
  void map_put_item(int fd);
  int  map_get_item(int fd);
  void map_delete_item_fd(int fd); 
  void map_delete_item(int index);

  void map_put_item_server(int fd, const CHARSTRING& local_IP_address, unsigned short local_port);
  int  map_get_item_server(int fd);
  void map_delete_item_fd_server(int fd); 
  void map_delete_item_server(int index);
  
  int create_socket(int addr_family);
  int fill_addr_struct(const char* name, int port, struct sockaddr_storage* sa, socklen_t& saLen);
  void setNonBlocking(int fd);
    
  boolean simple_mode;
  boolean reconnect;
  int reconnect_max_attempts;
  boolean server_mode;
  boolean debug;
  int server_backlog;
  CHARSTRING local_IP_address;
  CHARSTRING peer_IP_address;
  unsigned short local_port;
  unsigned short peer_port;

  struct sctp_event_subscribe events;
  struct sctp_initmsg  initmsg;
  
  boolean local_port_is_present;
  boolean peer_IP_address_is_present;
  boolean peer_port_is_present;

  int fd;

  int receiving_fd;

  struct fd_map_item;
  fd_map_item *fd_map;
  int list_len;

  struct fd_map_server_item;
  fd_map_server_item *fd_map_server;
  int list_len_server;


};
}
#endif

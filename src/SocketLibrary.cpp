#include "Library/Library.h"

SocketLogger::SocketLogger(const char* host_, const char* port_, MESSAGE_IMPORTANCE level) noexcept: BaseLogger(level), socket_ {-1} {
    
    memset(&hints, 0, sizeof(hints)); 
    hints.ai_family = AF_UNSPEC; 
    hints.ai_socktype = SOCK_STREAM;

    int status{};

    if ((status = getaddrinfo( host_, port_, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((socket_ = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(socket_, p->ai_addr, p->ai_addrlen) == -1) {
			perror("client: connect");
			close(socket_);
			continue;
		}

		break;
	}

	if (p == nullptr) {
		fprintf(stderr, "client: failed to connect\n");
	}

    if (p != nullptr) {
        inet_ntop(p->ai_family, get_in_addr(),
			s, sizeof s);
	printf("client: connecting to %s\n", s);
    }
    

    freeaddrinfo(servinfo);
    servinfo = nullptr;
}

SocketLogger::~SocketLogger() {
    close(socket_);
}

void SocketLogger::addMessageToLog(MESSAGE_IMPORTANCE level, std::string& message, std::tm* time) {
    if (level < level_)
        return;
    
    const char* new_message = message.c_str();

    if (socket_ > -1) {
        if (send(socket_, new_message, std::strlen(new_message), 0) == -1) 
            perror("send");
    }
}

void* SocketLogger::get_in_addr()
{
    auto temp = static_cast<sockaddr*>(p->ai_addr);

    if (temp->sa_family == AF_INET) {
        return &((reinterpret_cast<sockaddr_in*>(temp))->sin_addr);
    }
    return &((reinterpret_cast<sockaddr_in6*>(temp))->sin6_addr);
}
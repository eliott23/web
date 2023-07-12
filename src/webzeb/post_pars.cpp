#include "server.hpp"

// ********************** POST METHOD 
void    string_split(std::string& m, std::string& s1, std::string& s2, std::string& lim)
{
    size_t h_l = 0;

    if ((h_l = m.find(lim)) != std::string::npos)
    {
        s1 += m.substr(0, h_l);
        s2 += m.substr(h_l + lim.size());
    }
    // else
    // {
    //     perror("delimiter not found:");
	// 	throw std::runtime_error(strerror(errno));
    // }
}

void    server::split_head_body(char *buffer, pars &p)
{
    std::string sbuff(buffer, p.valread);
    std::string lim = "\r\n\r\n";
    string_split(sbuff, p.header, p.body_chunk, lim);
}

int r_err(ssize_t d)
{
    if (d < 0)
    {
        perror("error reading from socket");
		throw std::runtime_error(strerror(errno));
    }
    if (!d)
    {
        perror("server hanging while reading form socket");
		throw std::runtime_error(strerror(errno));
    }
    return (1);
}

void	server::post_parse(pars& p)
{
    //add the checks here
    /********/
    /********/
    /********/
    std::ofstream   outp(UPLOADED_FILE);
    if (!outp)
    {
        perror("failed openning file");
		throw std::runtime_error(strerror(errno));
    }
    size_t  w = 0;
    char    buffer[1024] = {0};

    if (p.body_chunk.size())
    {
        outp.write(UPLOADED_FILE, p.body_chunk.size());
        p.body_chunk.clear();
    }
    while (w <= FILE_SIZE)
    {
        p.valread = read(clientSocket, buffer, 1024);
        r_err(p.valread);
        w += p.valread;
        p.upd_valread();
        //check if its chunked later;
        outp.write(buffer, p.valread);
        outp.flush();
    }
    std::cout << "aaand it went OUT" << "size==" << p.t_valread << std::endl;
    //send HTTP OK 2000 here;
    outp.close();
}

pars::pars()
{
    type = 0; 
    t_valread = 0;
    valread = 0;
    cont_l = 0;
    max = M_H;
    p_h = 0;
}

void	pars::upd_valread()
{
    t_valread += valread;
    if (t_valread > max)
    {
       perror("max value reached ===>"); 
       std::cout << max << std::endl;
       throw std::runtime_error(strerror(errno));
    }
}
// ****** END OF POST METHOD 
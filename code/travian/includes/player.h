#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <array>
#include <vector>
#include <iostream>
#include "connection.h"
#include "cmd_line.h"

class player
{
public:
    enum RESOURSE
    {
        WOOD = 0,
        CLAY = 1,
        IRON = 2,
        WHEAT = 3,
        CONSUMPTION = 4
    };

    struct building
    {
        enum TYPE
        {
            LUMBER = 0,
            CLAY_QUARRY,
            IRON_MINE,
            FARM,
            MAIN,
            HIDE,
            MILL,
            SAWMILL,
            BRICKYARD,
            IRON_FOUNDRY,
            BAKERY,
            STORAGE,
            BARN,
            EMBASSY,
            MARKET,
            RESIDENCE,
            PALACE,
            MASON,
            TREASURE,
            WATER,
            TOWN_HALL,
            BOARD_TRADE,
            BIG_BRAN,
            BIG_STORAGE,
            ASSEMBLY_POINT,
            WALL,
            TAVERN,
            ACADEMY,
            SMITHY,
            STABLE,
            WORKSHOP,
            ARENA,
            UNBUILD
        };

        static const std::vector<std::string>types;

        enum TYPE type = UNBUILD;
        unsigned level = 0;
        unsigned id = 0;
    };

    struct keys
    {
        std::string server;
        std::string login;
        std::string password;

        keys(const std::string& server, const std::string& login, const std::string& password) : server(server), login(login), password(password) { }
    };

    struct resourses
    {
        std::array<unsigned, 4> storage;
        std::array<unsigned, 5> production;
        std::array<unsigned, 4>  max_storage;
    };


//    struct event
//    {
//        enum TYPE
//        {
//            CHECK,
//            BUILD
//        };

//        typedef void (*function)();

//        enum TYPE type;

//    };
private:
    const std::string login = "login.php";
    const std::string domain = "dorf1.php";
    const std::string village = "dorf2.php";
    const std::string build_id ="build.php?id=";

    connection con;

    std::string page;

    std::string server;

    std::array<building, 40> domains;
    const size_t domain_range = 18;
    const size_t village_range = 40;

    resourses res;
    std::string build_button_id;

    typedef void (player::*execute)(const cmd_line&);

    struct cmd_info
    {
        std::string cmd;
        execute func;
    };

    static std::vector<cmd_info> cmds;

    void execute_info_resource(const cmd_line& cmd);
    void execute_exit(const cmd_line& cmd);

    void upgrade(building& upgrade);
    std::array<unsigned, 5> get_build_cost(const building& upgrade);
    building get_building_description(size_t id);
public:

    player(const keys& info);

    ~player() { }



    void run(void);

    void get_resourses(void);
    void get_build_button_id(void);
    void get_domain_info(void);
    void get_village_info(void);


    void print_domain_info(void);
    void print_village_info(void);

    bool check_building(player::building::TYPE type);
    void build(player::building::TYPE type);
    void run_domain_upgrade_strategy(void);

//	public function map()
//	{
//		if( !$this->connected )
//		{
//			throw new Exception('Can\'t access this page until you are connected');
//		}

//		return $this->handle->get_url_content($this->server_host.'karte.php');
//	}

//	/**
//	 * Return the informations about the square
//	 *
//	 * @param int $did karte.php?d=
//	 * @param int $cid karte.php?c=
//	 */
//	public function get_square($did, $cid='')
//	{
//		if( !$this->connected )
//		{
//			throw new Exception('Can\'t access this page until you are connected');
//		}

//		// If $c is not given, found it !
//		if( strlen($cid) == 0 )
//		{
//			$cid = $this->get_square_key($did);
//		}

//		// Now get the square !
//		$content = $this->handle->get_url_content($this->server_host.'karte.php?d='.$did.'&c='.$cid);

//		return $this->parse_square($did, $content);
//		return $return;
//	}
//	/**
//	 * Create a building
//	 *
//	 * @param int $pos build.php?id=
//	 * @param int $bid dorf2.php?a=
//	 * @param int $cid dorf2.php?c=
//	 */
//	public function create_building($pos, $bid, $cid='')
//	{
//		if( !$this->connected )
//		{
//			throw new Exception('Can\'t access this page until you are connected');
//		}
//		// If $c is not given, found it !
//		if( strlen($c) == 0 )
//		{
//			$c = $this->get_building_key($pos);
//		}
//		// Check if the building can be build and if, http://s5.travian.fr/dorf2.php?a=10&id=29&c=38b
//		// get 2 times the same page...
//	}
//	/******* Parse Functions ************/
//	/**
//	 * Parse a square and return informations about it
//	 *
//	 * @param int $d karte.php?id=
//	 * @param string $content Content of the page
//	 * @return array did => Identifier of the square, 'ressources' => List of the ressources in the square, 'animals' => Only if an oasis with animals, Array of animals, 'owner' => If there is an Owner, his identifier
//	 */
//	private function parse_square($d, $content)
//	{
//		$animals = $this->get_animals($content);
//		$owner = $this->get_owner($content);
//		$return = array(
//			'did' => $d,
//			'ressources' => $this->get_res($content),
//		);
//		if( !empty($animals) )
//			$return['animals'] = $animals;
//		if( !empty($owner) )
//			$return['owner'] = $owner;
//		return $return;
//	}

//	/******* Get Functions **************/
//	/**
//	 * Get the key of the square
//	 *
//	 * @param int $z Identifier of the square
//	 */
//	private function get_square_key($z)
//	{
//		$content = $this->handle->get_url_content($this->server_host.'karte.php?z='.$z);

//		preg_match('#d='.$z.'&c=([a-f0-9]+)#', $content, $match);
//		return $match[1];
//	}
//	/**
//	 * Get the key to build a building
//	 *
//	 * @param int $pos Identifier of the place to build
//	 */
//	private function get_building_key($pos)
//	{
//		$content = $this->handle->get_url_content($this->server_host.'build.php?id='.$pos);
//		preg_match('#id='.$pos.'&c=([a-f0-9]{3})#', $content, $match);
//		return $match[1];
//	}
//	// By the picture always works. By table don't work when the square is occuped
//	private function get_res($square_content)
//	{
//		$res = array();

//		// Determine the type of the square
//		// Wood, Clay, Iron, Crops
//		// <div id="f1"
//		$fields_types = array(
//			"1" => array(3,3,3,9),
//			"2" => array(3,4,5,6),
//			"3" => array(4,4,4,6),
//			"4" => array(4,5,3,6),
//			"5" => array(5,3,4,6),
//			"6" => array(1,1,1,15),
//			// Deactivated
//			//"7" => array(),
//			//"8" => array(),
//			//"9" => array(),
//			//"10" => array(),
//		);

//		// Determine the type of the oasis
//		// Wood, Clay, Iron, Crops
//		// <img src="img/un/m/w1.jpg
//		$oasis_types = array(
//			"1" => array(1,0,0,0),
//			"2" => array(1,0,0,0),
//			"3" => array(1,0,0,1),
//			"4" => array(0,1,0,0),
//			"5" => array(0,1,0,0),
//			"6" => array(0,1,0,1),
//			"7" => array(0,0,1,0),
//			"8" => array(0,0,1,0),
//			"9" => array(0,0,1,1),
//			"10" => array(0,0,0,1),
//			"11" => array(0,0,0,1),
//			"12" => array(0,0,0,2)
//		);

//		if( ! preg_match("#<div id=\"f(\d{1,2})#", $square_content, $match) )
//		{
//			if( preg_match("#<img src=\"img/un/m/w(\d{1,2}).jpg#", $square_content, $match))
//			{
//				$res = $oasis_types[$match[1]];
//			}
//		}
//		else
//		{
//			$res = $fields_types[$match[1]];
//		}

//		return $res;
//	}

//	private function get_animals($square_content)
//	{
//		// ID 1,2,3,4
//		// <tr>
//		// <td><img class="res" src="img/un/r/1.gif"></td>
//		// <td class="s7 b">3</td><td> Bois</td>
//		// </tr>
//		preg_match_all('#<img class="unit" src="img/un/u/(\d+)\.gif" border="0">.*?(\d+)#', $square_content, $matches);
//		return (!empty($matches[1]) ) ? array_combine($matches[1], $matches[2]) : array();
//	}

//	private function get_owner($square_content)
//	{
//		$owner = array();

//		if( preg_match('#<a href="spieler\.php\?uid=(\d+)"> <b>(.+?)</b>#s', $square_content, $match) )
//		{
//			$owner['uid'] = $match[1];
//			$owner['name'] = trim($match[2]);
//		}

//		if( preg_match('#<a href="allianz\.php\?aid=([1-9]\d+)">(.*?)</a>#s', $square_content, $match) )
//		{
//			$owner['ally']['aid'] = $match[1];
//			$owner['ally']['name'] = trim($match[2]);
//		}

//		// Oasis Only !
//		if( preg_match('#<a href="karte\.php\?d=(\d+)&c=\d+">(.+?)</a>#s', $square_content, $match) )
//		{
//			$owner['village']['d'] = $match[1];
//			$owner['village']['name'] = trim($match[2]);
//		}

//		return $owner;
//    }
//}

};

#endif // PLAYER_H

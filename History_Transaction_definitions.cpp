#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 1
//
Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date,  unsigned int month_date,  unsigned year_date, 
        bool buy_sell_trans,  unsigned int number_shares,  
        double trans_amount ){

      
  this->symbol = ticker_symbol;
  this->day = day_date;
  this->month = month_date;
  this->year = year_date;
  if (buy_sell_trans){
    this->trans_type = "Buy";
  }
  else{
    this->trans_type = "Sell";
  }
  this->shares = number_shares;
  this->amount = trans_amount;
  this->trans_id = assigned_trans_id;
  ++assigned_trans_id;

  this->acb = 0.0;
  this->acb_per_share = 0.0;
  this->share_balance = 0;
  this->cgl = 0.0;

  this->p_next = nullptr; 

}


// Destructor
// TASK 1
//
Transaction::~Transaction(){

  
}

// Overloaded < operator.
// TASK 2
//
bool Transaction::operator<( Transaction const &other ){

  if (this->year < other.get_year()){
    return true;
  } 
  else if (this->year == other.get_year()) {
    if(this->month < other.get_month()){
      return true;
    }
    else if (this->month == other.get_month()){
      if( (this->day < other.get_day()) ){
        return true;
      }
    }
  }

  return false;

}

// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
	//
  History::History(){

    this->p_head = nullptr;

  }

	// Destructor.
	//
	History::~History(){
    
    while ( this->p_head != nullptr ){
      Transaction *prev_node = this->p_head;
      this->p_head = this->p_head->get_next();
      delete prev_node;
      prev_node = nullptr; 
    }


  }

	// Read the transaction history from file.
	//
	void History::read_history(){
    
    ece150::open_file();

    while (ece150::next_trans_entry()){


      Transaction *new_trans{ new Transaction{ ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), ece150::get_trans_year(),
                                                ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount() } };
      
      insert(new_trans);
    }

    ece150::close_file();

  }

	// Insert transaction in linked list.
	//
	void History::insert(Transaction *p_new_trans){

    Transaction *temp{this->p_head};

    if (this->p_head == nullptr){
      this->p_head = p_new_trans; 

    } else {
      while( temp->get_next() != nullptr ){
        temp = temp->get_next();
      }

      temp->set_next(p_new_trans); 
    }

  temp = nullptr;
  p_new_trans = nullptr;

 }
  

	// Sort using insertion sort.
	//
	void History::sort_by_date(){

    Transaction *new_p_head{nullptr};
    Transaction *p_front{nullptr};

    while (this->p_head != nullptr){

      //isolating the front node
      p_front = this->p_head;
      this->p_head = this->p_head->get_next();
      p_front->set_next(nullptr);

      if(new_p_head == nullptr){   //first entry for sorted linked list
        new_p_head = p_front;
        p_front = nullptr; 
        //std::cout << "first" << std::endl;
      } 
      else{  //other entries 

        //std::cout << "second" << std::endl;
        //inserting at the head
        if(*p_front < *new_p_head){
          p_front->set_next(new_p_head);
          new_p_head = p_front;
          p_front = nullptr;
          //std::cout << "third" << std::endl;

        }
        else{
          Transaction *p_temp{new_p_head};
          //std::cout << "fourth" << std::endl;
         // if(p_temp->get_next() == nullptr){
            //do nothing; temp is already in the right position
         // }
         // else{
            while( (p_temp->get_next() != nullptr) && (*(p_temp->get_next()) < *p_front) ){
              //std::cout << "loop" << std::endl; 
              p_temp = p_temp->get_next();
            }
          //}
          //insert
          p_front->set_next(p_temp->get_next());
          p_temp->set_next(p_front);
          p_front = nullptr;
          p_temp = nullptr;
          //std::cout << "fifth" << std::endl;

        }
      }

    } 

    this->p_head = new_p_head; 
    new_p_head = nullptr;

  }

	// Update the acb members in the linked list.
	//
	void History::update_acb_cgl(){

    Transaction *temp{this->p_head};
    Transaction *prev{this->p_head};

    while (temp != nullptr){
    
      if (temp == this->p_head){
        //std::cout << "loop1" << std::endl;
        //acb
        temp->set_acb(temp->get_amount());
        //acb per share
        temp->set_acb_per_share( (temp->get_acb()) / (temp->get_shares()) );
        //share balance
        temp->set_share_balance(temp->get_shares());
        //cgl
        temp->set_cgl(0.0); 
      }
      else{
        
        //std::cout << "loop2 WIOd" << std::endl;
        if(temp->get_trans_type()){
          temp->set_acb( prev->get_acb() + temp->get_amount() ); //acb
          temp->set_share_balance( prev->get_share_balance() + temp->get_shares() ); //share balance
          temp->set_acb_per_share( (temp->get_acb()) / (temp->get_share_balance()) ); //acb_per_share
          temp->set_cgl(0.0); //cgl
        }
        else{
          temp->set_acb( prev->get_acb() - ((temp->get_shares()) * (prev->get_acb_per_share()))); //acb
          temp->set_acb_per_share( prev->get_acb_per_share() ); //acb_per_share
          temp->set_share_balance( prev->get_share_balance() - temp->get_shares() ); //share balance
          temp->set_cgl( temp->get_amount() - ((temp->get_shares()) * (temp->get_acb_per_share())) ); //clg
        }

      }

      prev = temp;
      temp = temp->get_next();
    }

  temp = nullptr;
  prev = nullptr;

  }

	// Compute the ACB, and CGL.
	//
	double History::compute_cgl(unsigned int year){

    Transaction *temp{this->p_head};
    double cgl{0.0};

    while(temp != nullptr){
      if(temp->get_year() == year){
        cgl += temp->get_cgl();
      }
      temp = temp->get_next();
    }

    temp = nullptr;

    return cgl;
  }  

	// Print the transaction history.
	//
	void History::print(){

    Transaction *p_temp{this->p_head};

    std::cout << "========= BEGIN TRANSACTION HISTORY ============" << std::endl;

    while(p_temp != nullptr){
      p_temp->print();
      p_temp = p_temp->get_next();
    }


    std::cout << "========= END TRANSACTION HISTORY ============" << std::endl;

    p_temp = nullptr;

  }



// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }

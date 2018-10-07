#include <fc/variant.hpp>
#include <eosio/wallet_plugin/wallet_manager.hpp>

#include <Python.h>

#include "../pyeoskit/pyobject.hpp"

using namespace std;
using namespace eosio;
using namespace eosio::chain;
using namespace eosio::wallet;

extern "C" PyObject* PyInit_wallet();

PyObject* init_wallet() {
   printf("+++++init wallet.\n");
   return PyInit_wallet();
}

wallet_manager& wm() {
   static wallet_manager* wm = nullptr;
   if (!wm) {
      wm = new wallet_manager();
   }
   return *wm;
}

void sign_transaction(signed_transaction& trx) {
#if 0
   const auto& public_keys = wm().get_public_keys();
   auto required_keys_set = get_chain_controller().get_authorization_manager().get_required_keys( trx, public_keys, fc::seconds( 10 ));
   trx = wm().sign_transaction(trx, required_keys_set, get_chain_controller().get_chain_id());
#endif
}

PyObject* wallet_create_(std::string& name) {
   string password = "";
   try {
      password = wm().create(name);
   } catch (fc::exception& ex) {
      elog(ex.to_detail_string());
   } catch (...) {
   }
   return py_new_string(password);
}

PyObject* wallet_open_(std::string& name) {
   try {
      wm().open(name);
   } catch (fc::exception& ex) {
      elog(ex.to_detail_string());
      return py_new_bool(false);
   } catch (...) {
      return py_new_bool(false);
   }
   return py_new_bool(true);
}

PyObject* wallet_set_dir_(std::string& path_name) {
   try {
      wm().set_dir(path_name);
   } catch (fc::exception& ex) {
      elog(ex.to_detail_string());
      return py_new_bool(false);
   } catch (...) {
      return py_new_bool(false);
   }
   return py_new_bool(true);
}

PyObject* wallet_set_timeout_(uint64_t secs) {
   try {
      wm().set_timeout(secs);
   } catch (fc::exception& ex) {
      elog(ex.to_detail_string());
      return py_new_bool(false);
   } catch (...) {
      return py_new_bool(false);
   }
   return py_new_bool(true);
}

/*
 string sign_transaction(txn,keys,id){
 #    const chain::signed_transaction& txn, const flat_set<public_key_type>&
 keys,const chain::chain_id_type& id
 }
 */

PyObject* wallet_list_wallets_() {
   PyArray arr;
   try {
      auto list = wm().list_wallets();
      for (auto it = list.begin(); it != list.end(); it++) {
         arr.append(*it);
      }
   } catch (fc::exception& ex) {
      elog(ex.to_detail_string());
   } catch (...) {
   }
   return arr.get();
}

PyObject* wallet_list_keys_(const string& name, const string& pw) {
   PyDict results;
   try {
      map<public_key_type, private_key_type> keys = wm().list_keys(name, pw);
      variant v;
      for (auto it = keys.begin(); it != keys.end(); it++) {
         //            to_variant(it.first,v);
         //            results.add(v.as_string(),key_value.second);
         string key = string(it->first);
         string value = string(it->second);
         results.add(key, value);
      }
   } catch (fc::exception& ex) {
      elog(ex.to_detail_string());
   } catch (...) {
   }
   return results.get();
}

PyObject* wallet_get_public_keys_() {
   PyArray results;

   try {
      flat_set<public_key_type> keys = wm().get_public_keys();
      //        variant v;
      for (auto it = keys.begin(); it < keys.end(); it++) {
         //                to_variant(*it,v);
         //                results.append(v.as_string());
         results.append((string)*it);
      }
   } catch (fc::exception& ex) {
      elog(ex.to_detail_string());
   } catch (...) {
   }
   return results.get();
}

PyObject* wallet_lock_all_() {
   try {
      wm().lock_all();
   } catch (fc::exception& ex) {
      elog(ex.to_detail_string());
      return py_new_bool(false);
   } catch (...) {
      return py_new_bool(false);
   }
   return py_new_bool(true);
}

PyObject* wallet_lock_(string& name) {
   try {
      wm().lock(name);
   } catch (fc::exception& ex) {
      elog(ex.to_detail_string());
      return py_new_bool(false);
   } catch (...) {
      return py_new_bool(false);
   }
   return py_new_bool(true);
}

PyObject* wallet_unlock_(string& name, string& password) {
   try {
      wm().unlock(name, password);
   } catch (fc::exception& ex) {
      elog(ex.to_detail_string());
      return py_new_bool(false);
   } catch (...) {
      return py_new_bool(false);
   }
   return py_new_bool(true);
}

PyObject* wallet_import_key_(string& name, string& wif_key, bool save) {
   try {
      wm().import_key(name, wif_key, save);
   } catch (fc::exception& ex) {
      elog(ex.to_detail_string());
      return py_new_bool(false);
   } catch (...) {
      return py_new_bool(false);
   }
   return py_new_bool(true);
}

PyObject* wallet_save_(string& name) {
   try {
      wm().save_wallet(name);
   } catch (fc::exception& ex) {
      elog(ex.to_detail_string());
      return py_new_bool(false);
   } catch (...) {
      return py_new_bool(false);
   }
   return py_new_bool(true);
}
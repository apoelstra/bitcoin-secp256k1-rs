extern crate secp256k1;

use secp256k1::rand::rngs::OsRng;
use secp256k1::{PublicKey, Secp256k1, SecretKey};

fn main() {
    let secp = Secp256k1::new();
    // First option:
    let (seckey, pubkey) = secp.generate_keypair(&mut OsRng);

    assert_eq!(pubkey, PublicKey::from_secret_key(&secp, &seckey));

    // Second option:
    let seckey = SecretKey::new(&mut OsRng);
    let _pubkey = PublicKey::from_secret_key(&secp, &seckey);
}

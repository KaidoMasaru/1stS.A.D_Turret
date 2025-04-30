// --------------------------------
// クラス名：Automaticクラス
// 概要　　：防衛設備を自動で動くようにするコン??ネント
// 作成者　：23cu0207 海藤 縁
// 作成日　：2024/12/02
// 更新日　：2024/12/23	コリジョン処理を作成
//		　 　2025/12/28 敵がいる方向のベクトル計算処理を作成
//		　 　2025/01/07 敵の方向を向く処理を作成
//		　 　2025/01/14 弾丸の発射処理を作成
//			 2025/01/20 EnemyListの取得Tagを修正
//			 2025/01/24 コリジョン終了時の処理を作成
//			 2025/01/31 EnemyList内の情報処理を適切に動くよう修正
// --------------------------------

#include "Building/Automatic.h"
#include "Components/SphereComponent.h"
#include "Attack/AttackBase.h"
#include "Attack/BulletMovement.h"
#include "Components/SceneComponent.h"


// コンストラクタ
UAutomatic::UAutomatic()
{
	// 球体のコリジョンを作成
	m_SearchingArea = CreateDefaultSubobject<USphereComponent>(TEXT("SearchingArea"));

	// m_SearchingAreaのNullチェック
	if (m_SearchingArea)
	{
		// コリジョン設定を明示的に設定
		m_SearchingArea->SetGenerateOverlapEvents(true);
		m_SearchingArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		m_SearchingArea->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		m_SearchingArea->SetCollisionResponseToAllChannels(ECR_Overlap);
		
		// OnComponentBeginOverlapをBindする
		m_SearchingArea->OnComponentBeginOverlap.AddDynamic(this, &UAutomatic::OnSphereBeginOverlap);
		UE_LOG(LogTemp, Warning, TEXT("OnComponentBeginOverlap bound successfully"));

		// OnComponentEndOverlapをBindする
		m_SearchingArea->OnComponentEndOverlap.AddDynamic(this, &UAutomatic::OnSphereEndOverlap);
		UE_LOG(LogTemp, Warning, TEXT("OnComponentEndOverlap bound successfully"));
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("m_SearchingArea: Null"));
	}
}

//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
// 感知範囲のコリジョンに当たった時の処理
// 引数：第一引数：UPrimitiveComponent* OverlappedComponent
//		 第二引数：AActor* OtherActor
//		 第三引数：UPrimitiveComponent* OtherComp
//		 第四引数：int32 OtherBodyIndex
//		 第五引数：bool bFromSweep
//		 第六引数：FHitResult& SweepResult
// 戻り値：なし
//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
void UAutomatic::OnSphereBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// コリジョンしたActorが敵か判定する
	if (OtherActor->ActorHasTag("Enemy"))
	{
		// コリジョンしたオブジェクトの情報をリストに追加する
		EnemyList.AddUnique(OtherActor);
	}

}

//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
// 感知範囲のコリジョン終了時の処理
// 引数：第一引数：UPrimitiveComponent* OverlappedComponent
//		 第二引数：AActor* OtherActor
//		 第三引数：UPrimitiveComponent* OtherComp
//		 第四引数：int32 OtherBodyIndex
// 戻り値：なし
//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
void UAutomatic::OnSphereEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("OnSphereEndOverlap Function"));

	if (!OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("OtherActor:NULL"));
	}

	//　コリジョンが終了したアクターのタグがエネミーのとき
	if (OtherActor && OtherActor->ActorHasTag("Enemy"))
	{
		// デバッグ用
		// リストから削除する前にサイズを記録
		int32 BeforeSize = EnemyList.Num();

		// コリジョンが終了した敵をリストから削除
		EnemyList.Remove(OtherActor); 

		// 削除されたか確認
		if (EnemyList.Num() < BeforeSize)
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy Removed from List: %s"), *OtherActor->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to remove enemy from list: %s"), *OtherActor->GetName());
		}
	}
}

//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
// 敵がいる方向を計算する処理
// 引数：第一引数：FVector TargetLocation
//		 第二引数FVector MyLocation
// 戻り値：FRotator型
//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
FRotator UAutomatic::RotateVec(FVector TargetLocation, FVector MyLocation)
{
	// return用変数
	FRotator ret = FRotator(0.0, 0.0, 0.0);

	// タレットの現在の位置とターゲットの位置から方向を計算
	FVector Direction = TargetLocation - MyLocation;

	// 方向ベクトルの長さが 0 でないことを確認
	if (!Direction.IsNearlyZero())
	{
		ret = FRotationMatrix::MakeFromX(Direction).Rotator();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RotateObject:Zero"));
	}
	return ret;
}

//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
// 弾丸の発射処理
// 引数：なし
// 戻り値：なし
//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
void UAutomatic::FiringBullet()
{
	// Ownerを変数に保存
	AActor* Owner = GetOwner();

	// 弾の生成位置を設定
	// タレットの発射口がある座標を取得
	FVector SpawnLocation = m_Mesh->GetSocketLocation("muzzle");

	// OwnerのNULLチェック
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner:Null"));
		return;
	}

	// 弾オブジェクトを生成
	AAttackBase* Bullet = GetWorld()->SpawnActor<AAttackBase>(m_Bullet, SpawnLocation, GetOwner()->GetActorRotation());

	// BulleのNullチェック
	if(!Bullet)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bullet:Null"));
		return;
	}

	// 弾オブジェクトが飛ぶ方向を設定
	UBulletMovement* Movement = Bullet->FindComponentByClass<UBulletMovement>();
	
	// Nullチェック
	if (Movement)
	{
		// 弾丸の発射方向を設定
		Movement->SetMoveDirection(GetEnemyLocation());
		// 弾丸の飛ぶ速さを設定
		Movement->SetMoveSpeed(10.0);
		// 発射フラグをログに表示（デバッグ用）
		UE_LOG(LogTemp, Warning, TEXT("Turret:Fire"));
	}

	else
	{
		// デバッグログ
		UE_LOG(LogTemp, Warning, TEXT("Movement:Null"));
	}
}

//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
// EnemyListの先頭に入っている敵情報の座標を取得する処理
// 引数：なし
// 戻り値：FVector型
//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
FVector UAutomatic::GetEnemyLocation()
{
	// return用の変数
	FVector ret = FVector(0.0,0.0,0.0);

	// Nullチェック
	if (EnemyList[0] != nullptr)
	{
		// EnemyList[0]に入っているActor情報から座標を取得
		ret = EnemyList[0]->GetActorLocation();
	}

	return ret;
}

//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
// タレットのメッシュを設定する処理（BPで設定する用）
// 引数：第一引数：USceneComponent* SceneComp
// 戻り値：なし
//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
void UAutomatic::SetSceneComponent(USceneComponent* SceneComp)
{
	m_Mesh = SceneComp;
}

//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
// EnemyListに値が入っているか確認する処理（値が入っていればtrue）
// 引数：なし
// 戻り値：bool型
//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
bool UAutomatic::EnemyListIsValid()
{
	// return用変数
	bool ret = false;

	if (EnemyList.Num() > 0)
	{
		ret = true;
	}
	return ret;
}

//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
// EnemyListでNullPtrになった要素を全て削除する処理
// 引数：なし
// 戻り値：なし
//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
void UAutomatic::RemoveListNullPtr()
	{
		EnemyList.RemoveAll([](const TWeakObjectPtr<AActor>& WeakActor)
			{
				return !WeakActor.IsValid();  // 無効な要素を削除
			});
	}
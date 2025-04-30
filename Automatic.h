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
#pragma once

#include "CoreMinimal.h"
#include "Building/ControlType.h"
#include "Automatic.generated.h"

/**
 *
 */

 // 前方宣言
class USphereComponent;		// 球体コリジョンのコンポーネント
class USceneComponent;		// スタティックメッシュ情報の保存用
class AAttackBase;			// 発射物（弾丸等）の親クラス
class UBulletMovement;		// 弾丸の移動コンポーネント


UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class SAD_API UAutomatic : public UControlType
{
	GENERATED_BODY()
public:
	UAutomatic();	// コンストラクタ

	// 感知範囲用のの球体コリジョン 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Automatic)
	TObjectPtr<USphereComponent> m_SearchingArea;

	// タレットのメッシュ情報
	TObjectPtr<USceneComponent> m_Mesh;	


	
protected:
	// 弾丸のクラス情報
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Automatic)
	TSubclassOf<AAttackBase> m_Bullet;

	//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
	// タレットのメッシュを設定する処理（BPで設定する用）
	// 引数：第一引数：USceneComponent* SceneComp
	// 戻り値：なし
	//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
	UFUNCTION(BlueprintCallable)
	void SetSceneComponent(USceneComponent* SceneComp);

private:
	TArray<TWeakObjectPtr<AActor>>EnemyList;// 感知した敵のリスト

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
	UFUNCTION(BlueprintCallable)
	void OnSphereBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
	// 感知範囲のコリジョン終了時の処理
	// 引数：第一引数：UPrimitiveComponent* OverlappedComponent　
	//		 第二引数：AActor* OtherActor
	//		 第三引数：UPrimitiveComponent* OtherComp　
	//		 第四引数：int32 OtherBodyIndex
	// 戻り値：なし
	//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
	UFUNCTION(BlueprintCallable)
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
	// 敵がいる方向を計算する処理
	// 引数：第一引数：FVector TargetLocation
	//		 第二引数：FVector MyLocation
	// 戻り値：FRotator型
	//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
	UFUNCTION(BlueprintCallable)
	FRotator RotateVec(FVector TargetLocation, FVector MyLocation);

	//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
	// 弾丸の発射処理
	// 引数：なし
	// 戻り値：なし
	//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ－
	UFUNCTION(BlueprintCallable)
	void FiringBullet();

	//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
	// EnemyListの先頭に入っている敵情報の座標を取得する処理
	// 引数：なし
	// 戻り値：FVector型
	//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
	UFUNCTION(BlueprintCallable)
	FVector GetEnemyLocation();

	//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
	// EnemyListに値が入っているか確認する処理（値が入っていればtrue）
	// 引数：なし
	// 戻り値：bool型
	//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
	UFUNCTION(BlueprintCallable)
	bool EnemyListIsValid();

	//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
	// EnemyListでNullPtrになった要素を全て削除する処理
	// 引数：なし
	// 戻り値：なし
	//ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
	UFUNCTION(BlueprintCallable)
	void RemoveListNullPtr();
};
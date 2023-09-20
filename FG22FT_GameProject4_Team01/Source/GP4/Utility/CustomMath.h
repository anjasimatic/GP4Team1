#pragma once

struct FCustomMath
{
	/** Custom InterpTo that clamps min distance*/
	UE_NODISCARD static FVector VInterpTo( const FVector& Current, const FVector& Target, float DeltaTime, float InterpSpeed, const float MinDist = 10.f)
	{
		// If no interp speed, jump to target value
		if( InterpSpeed <= 0.f )
		{
			return Target;
		}

		// Distance to reach
		const FVector Dist = Target - Current;

		// If distance is too small, just set the desired location
		if( Dist.SizeSquared() < UE_KINDA_SMALL_NUMBER )
		{
			return Target;
		}

		const FVector::FReal DeltaM = Dist.Size();
		const FVector DeltaMove = Dist.GetClampedToSize(MinDist, UE_BIG_NUMBER) * (DeltaTime * InterpSpeed);

		if(DeltaM < DeltaMove.Size()) return Target;

		return Current + DeltaMove;
	}
	
	/** Interpolate angle from Current to Target with constant step */
	template<typename T1, typename T2 = T1, typename T3 = T2, typename T4 = T3>
	UE_NODISCARD static auto FInterpAngleConstantTo(T1 Current, T2 Target, T3 DeltaTime, T4 InterpSpeed)
	{
		using RetType = decltype(T1() * T2() * T3() * T4());

		// if DeltaTime is 0, do not perform any interpolation (Location was already calculated for that frame)
		if( DeltaTime == 0.f || Current == Target )
		{
			return static_cast<RetType>(Current);
		}
	
		const RetType Dist = DeltaAngle(Current, Target);

		// If distance is too small, just set the desired location
		if( FMath::Square(Dist) < UE_SMALL_NUMBER )
		{
			return static_cast<RetType>(Target);
		}

		const RetType Step = InterpSpeed * DeltaTime;

		return Current + FMath::Clamp(Dist, -Step, Step);
	}

	/** Interpolate angle from Current to Target. Scaled by distance to Target, so it has a strong start speed and ease out. */
	template<typename T1, typename T2 = T1, typename T3 = T2, typename T4 = T3>
	UE_NODISCARD static auto FInterpAngleTo( T1 Current, T2 Target, T3 DeltaTime, T4 InterpSpeed )
	{
		using RetType = decltype(T1() * T2() * T3() * T4());

		// if DeltaTime is 0, do not perform any interpolation (Location was already calculated for that frame)
		if( DeltaTime == 0.f || Current == Target )
		{
			return static_cast<RetType>(Current);
		}
		
		// If no interp speed, jump to target value
		if( InterpSpeed <= 0.f )
		{
			return static_cast<RetType>(Target);
		}

		// Distance to reach
		const RetType Dist = DeltaAngle(Current, Target);

		// If distance is too small, just set the desired location
		if( FMath::Square(Dist) < UE_SMALL_NUMBER )
		{
			return static_cast<RetType>(Target);
		}

		// Delta Move, Clamp so we do not over shoot.
		const RetType DeltaMove = Dist * FMath::Clamp<RetType>(DeltaTime * InterpSpeed, 0.f, 1.f);

		return Current + DeltaMove;
	}

	// Calculates the shortest difference between two given angles.
	template<typename T1, typename T2, TEMPLATE_REQUIRES(TOr<TIsFloatingPoint<T1>, TIsFloatingPoint<T2>>::Value)>	
	UE_NODISCARD static auto DeltaAngle(T1 Current, T2 Target) -> decltype(Current * Target)
	{
		auto Delta = FMath::Fmod(Target - Current, 360.f);
		if (Delta > 180.f)
			Delta -= 360.f;
		else if (Delta < -180.f)
			Delta += 360.f;
		
		return Delta;
	}
};

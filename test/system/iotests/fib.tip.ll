; ModuleID = 'iotests/fib.tip'
source_filename = "iotests/fib.tip"
target triple = "x86_64-apple-darwin20.5.0"

@_tip_ftable = internal constant [2 x i64 ()*] [i64 ()* bitcast (i64 (i64)* @fib to i64 ()*), i64 ()* @_tip_main]
@_tip_num_inputs = constant i64 1
@_tip_input_array = common global [1 x i64] zeroinitializer

; Function Attrs: nounwind readnone willreturn
declare void @llvm.donothing() #0

define i64 @fib(i64 %n) {
entry:
  br label %header1

header1:                                          ; preds = %body1, %entry
  %f1.0 = phi i64 [ 1, %entry ], [ %f2.0, %body1 ]
  %f2.0 = phi i64 [ 1, %entry ], [ %addtmp, %body1 ]
  %i.0 = phi i64 [ %n, %entry ], [ %subtmp, %body1 ]
  %gttmp = icmp sgt i64 %i.0, 1
  br i1 %gttmp, label %body1, label %exit1

body1:                                            ; preds = %header1
  %addtmp = add i64 %f2.0, %f1.0
  %subtmp = add i64 %i.0, -1
  br label %header1

exit1:                                            ; preds = %header1
  ret i64 %f2.0
}

define i64 @_tip_main() {
entry:
  %tipinput0 = load i64, i64* getelementptr inbounds ([1 x i64], [1 x i64]* @_tip_input_array, i64 0, i64 0), align 4
  %calltmp = call i64 @fib(i64 %tipinput0)
  ret i64 %calltmp
}

; Function Attrs: nounwind
declare noalias i8* @calloc(i64, i64) #1

attributes #0 = { nounwind readnone willreturn }
attributes #1 = { nounwind }
